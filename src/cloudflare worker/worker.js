addEventListener('fetch', event => {
  event.respondWith(handleRequest(event))
})

async function handleRequest (event) {
  const parsedUrl = new URL(event.request.url)

  if (event.request.method === 'POST' && parsedUrl.pathname === '/upload') {
    return handleUpload(event.request)
  }

  if (event.request.method === 'GET' && parsedUrl.pathname === '/latest.jpg') {
    return handleLatest(event.request)
  }

  return new Response('Hello world')
}

async function handleUpload (request) {
  let imageData = await request.arrayBuffer()
  let start = await findFileStartOffset(imageData)

  await NAMESPACE.put('latest', imageData.slice(start, -58))
  return new Response('Success!')
}

async function handleLatest (request) {
  return new Response(
    await NAMESPACE.get('latest', 'arrayBuffer'),
    { headers: new Headers([['Content-Type', 'image/jpeg']]) }
  )
}

async function findFileStartOffset (imageData) {
  let decoder = new TextDecoder('utf-8')
  let start = 0
  let newlines = 0
  while (true) {
    let nextValue = decoder.decode(imageData.slice(start, start + 2))
    if (nextValue === '\r\n') {
      newlines += 1
    }
    if (newlines === 4) {
      return start + 2
    }
    start += 1
  }
}
