addEventListener("fetch", event => {
  event.respondWith(handleRequest(event))
})

async function handleRequest(event) {
    const parsedUrl = new URL(event.request.url)

    if (event.request.method === "POST" && parsedUrl.pathname === "/upload") {
        return handleUpload(event.request)
    }

    if (event.request.method === "GET" && parsedUrl.pathname === "/latest.jpg") {
        return handleLatest(event.request)
    }

    return new Response("Hello world")
}

async function handleUpload(request) {
    let imageData = await request.arrayBuffer()
    await NAMESPACE.put("latest", imageData)
    return new Response("Success!");
}

async function handleLatest(request) {
    let imageData = await NAMESPACE.get("latest")
    return new Response(
        imageData.slice(162, -58),
        { headers: new Headers([["Content-Type", "image/jpeg"]]) }
    )
}