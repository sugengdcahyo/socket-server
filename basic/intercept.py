from mitmproxy import http


def request(flow: http.HTTPFlow) -> None:
    if flow.request.host == "imgbox2" and flow.request.port == 8080:
        # Modifikasi pesan di sini
        modified_message = "Pesan telah dirubah"
        flow.request.content = modified_message.encode()


def response(flow: http.HTTPFlow) -> None:
    if flow.request.host == "imgbox2" and flow.request.port == 8080:
        # Modifikasi response jika diperlukan
        modified_response = "Response telah dirubah"
        flow.response.content = modified_response.encode()
