def request(flow):
    if flow.request.host == "imgbox2" and flow.request.port == 8080:
        # Modifikasi pesan jika perlu
        flow.request.headers["X-Modified"] = "MITM Proxy"
        flow.request.content = flow.request.content.replace(b"Hello", b"Intercepted")

