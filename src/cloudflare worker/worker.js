/* globals fetch, addEventListener, Headers, Response, NAMESPACE */
// Map to https://robertwallhead.com/crashy/*
addEventListener('fetch', event => {
  event.respondWith(handleRequest(event))
})

async function handleRequest (event) {
  const parsedUrl = new URL(event.request.url)

  if (event.request.method === 'POST' && parsedUrl.pathname === '/crashy/upload') {
    const authKey = await NAMESPACE.get('upload_key')
    if (authKey === event.request.headers.get('auth')) {
      return handleUpload(event.request)
    }
    return new Response('No')
  }

  if (event.request.method === 'GET' && parsedUrl.pathname === '/crashy/latest.jpg') {
    return handleLatest(event.request)
  }

  if (event.request.method === 'GET' && parsedUrl.pathname.startsWith('/crashy/')) {
    const relativePath = parsedUrl.pathname.slice('/crashy/'.length)
    return fetch(`https://thisismyrobot.github.io/crashy/${relativePath}`)
  }

  return new Response('Hello world')
}

async function handleUpload (request) {
  const imageData = await request.arrayBuffer()
  const start = await findFileStartOffset(imageData)
  const endOffset = await findFileEndOffset(imageData)

  await NAMESPACE.put('latest', imageData.slice(start, -endOffset))
  await NAMESPACE.put(`history_${new Date().toISOString()}.jpg`, imageData.slice(start, -endOffset))

  return new Response('Success!')
}

async function handleLatest (request) {
  return new Response(
    await NAMESPACE.get('latest', 'arrayBuffer'),
    { headers: new Headers([['Content-Type', 'image/jpeg']]) }
  )
}

async function findFileStartOffset (imageData) {
  const decoder = new TextDecoder('utf-8')
  let start = 0
  let newlines = 0
  while (true) {
    const nextValue = decoder.decode(imageData.slice(start, start + 2))
    if (nextValue === '\r\n') {
      newlines += 1
    }
    if (newlines === 4) {
      return start + 2
    }
    start += 1
  }
}

async function findFileEndOffset (imageData) {
  const decoder = new TextDecoder('utf-8')
  let start = 0
  while (true) {
    const nextValue = decoder.decode(imageData.slice(start, start + 2))
    if (nextValue === '\r\n') {
      break
    }
    start += 1
  }

  // 6 covers newlines and -- etc.
  return start + 6
}
