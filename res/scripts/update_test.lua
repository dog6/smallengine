local SCREEN_WIDTH  = 1280
local SCREEN_HEIGHT = 720

local RECT_WIDTH  = 50
local RECT_HEIGHT = 50

local LEFT   = RECT_WIDTH / 2
local RIGHT  = SCREEN_WIDTH - RECT_WIDTH / 2
local TOP    = RECT_HEIGHT / 2
local BOTTOM = SCREEN_HEIGHT - RECT_HEIGHT / 2

local SPEED = 200
local edge = 1
local myRectangle = nil

function Init()
    myRectangle = Entman.AddRect(
        "PatternRectangle",
        world_id,
        vec2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
        vec2f(RECT_WIDTH, RECT_HEIGHT),
        ColorRGB(255, 0, 0),
        true,
        false
    )

    if not myRectangle then
        print("Failed to create static rectangle")
    end
end

function update(dt)
    if not myRectangle then return end

    local pos = myRectangle:GetPosition()
    local step = SPEED * dt

    if edge == 1 then
        pos.x = math.min(pos.x + step, RIGHT)
        if pos.x >= RIGHT then edge = 2 end
    elseif edge == 2 then
        pos.y = math.min(pos.y + step, BOTTOM)
        if pos.y >= BOTTOM then edge = 3 end
    elseif edge == 3 then
        pos.x = math.max(pos.x - step, LEFT)
        if pos.x <= LEFT then edge = 4 end
    else
        pos.y = math.max(pos.y - step, TOP)
        if pos.y <= TOP then edge = 1 end
    end

    myRectangle:SetPosition(pos)
end

Init()