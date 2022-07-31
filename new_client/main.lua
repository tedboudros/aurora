local get = require("Requests.GET")
local draw_rounded_rect = require("Drawing.RoundedRect")

-- DEBUGGING START
if os.getenv("LOCAL_LUA_DEBUGGER_VSCODE") == "1" then
    require("lldebugger").start()
end
-- DEBUGGING STOP


local games = ''

local gameSize = 128
local gameMinMargin = 16
local gameBorderRadius = 8
local gameBorderSize = 3


screenMargin = 64

local gameStartX = screenMargin
local gameStartY = screenMargin


activeGameIndex = 10
local clickableCursor = love.mouse.getSystemCursor("hand")

function love.draw() 
    local ww = love.graphics.getWidth()
    local wh = love.graphics.getHeight()
    local usableWW = ww - (screenMargin * 2)
    local wLen = math.floor(usableWW / gameSize)
    local spaceLeft = (usableWW % gameSize)

    while ((spaceLeft / (wLen-1)) < gameMinMargin) do
        wLen = wLen - 1
        spaceLeft = spaceLeft + gameSize
    end

    local gameMargin = spaceLeft / (wLen-1)

    local hLen = math.ceil(#games / wLen)

    love.mouse.setCursor()

    for wi = 1, wLen do
        for hi = 1, hLen do
            local x = gameStartX + ((wi - 1) * (gameSize + gameMargin))
            local y = gameStartY + ((hi - 1) * (gameSize + gameMargin))
            local i = ((hi - 1) * wLen) + wi
            if (i > #games) then
                break
            end

            mouseX, mouseY = love.mouse.getPosition()
            if (mouseX > x and mouseX < x + gameSize and mouseY > y and mouseY < y + gameSize) then
                love.mouse.setCursor(clickableCursor)
                activeGameIndex = i
            end

            if(activeGameIndex == i) then
                love.graphics.setColor(87/255, 148/255, 235/255, 1)
                draw_rounded_rect(x, y, gameSize, gameSize, gameBorderRadius)
            end

            love.graphics.setColor(1, 1, 1, 1)
            draw_rounded_rect(x + gameBorderSize, y + gameBorderSize, gameSize - (gameBorderSize * 2), gameSize - (gameBorderSize * 2), gameBorderRadius)
        end
    end

    local barHeight = 64
    local textY = wh - barHeight + 18

    love.graphics.setColor(1, 1, 1, 1)
    love.graphics.rectangle("fill", 0, wh - barHeight, ww, barHeight)
    love.graphics.setColor(87/255, 148/255, 235/255, 1)
    love.graphics.print(games[activeGameIndex]["Name"], gameStartX, textY)
end

function love.load()
    local flags = {fullscreen = true, resizable=true, vsync = true, msaa=4}
    success = love.window.setMode( 800, 800, flags )

    font = love.graphics.newFont('Assets/Fonts/Oxygen-Bold.ttf', 24)

    love.graphics.setBackgroundColor(220/255, 220/255, 220/255, 1)
	love.graphics.setFont(font)

    
    games = get("http://localhost:40096/steam")
end