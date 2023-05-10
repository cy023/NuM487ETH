import http.client

conn = http.client.HTTPConnection("127.0.0.1", 80)
conn.request("GET", "/")
response = conn.getresponse()
body = response.read().decode()

print("Request URL: {}:{}\n".format(conn.host, conn.port))
print("HTTP/{} {} {}".format(response.version, response.status, response.reason))
for header in response.getheaders():
    print("{}: {}".format(header[0], header[1]))
print("\nBody:\n{}".format(body))

conn.close()
