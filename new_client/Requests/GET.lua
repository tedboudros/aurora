http = require("socket.http")
json = require('Parsers.JSON')

function get (url)
    local body, code, headers, status = http.request(url)
    return json.parse(body)
end

return get