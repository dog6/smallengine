-- Player movement settings
local PLAYER_SPEED = 300
local RECT_SIZE = vec2f(50, 50)

local myRectangle = nil

function Init()
    -- AddRect returns the created entity.
    -- Coordinates and size are specified in pixels.
    myRectangle = Entman.AddRect(
        "Player1",
        world_id,
        vec2f(400, 300),
        RECT_SIZE,
        ColorRGB(255, 0, 0),
        true,  -- Static body
        false   -- Pixel-based creation coordinates
    )
    
    if not myRectangle then
        print("Failed to create rectangle entity.")
        return
    end
    
    print("Player initialized.")
end

function Update(dt)
    if not myRectangle then
        return
    end
    
    local pos = myRectangle:GetPosition()
    
    -- Read movement input.
    local moveX = Keyboard.Axis(Key.A, Key.D, 1)
    local moveY = Keyboard.Axis(Key.W, Key.S, 1)
    
    -- Normalize diagonal movement so it isn't faster.
    if moveX ~= 0 and moveY ~= 0 then
        local diagonalScale = 0.70710678
        moveX = moveX * diagonalScale
        moveY = moveY * diagonalScale
    end
    
    local resolution = game_window.resolution
    
    -- Clamp the rectangle's center to keep it on-screen.
    local halfWidth = RECT_SIZE.x * 0.5
    local halfHeight = RECT_SIZE.y * 0.5

    pos.x = pos.x + moveX * PLAYER_SPEED * dt
    pos.y = pos.y + moveY * PLAYER_SPEED * dt

    pos.x = math.max(halfWidth,
        math.min(pos.x, resolution.x - halfWidth))

    pos.y = math.max(halfHeight,
        math.min(pos.y, resolution.y - halfHeight))

    myRectangle:SetPosition(pos)

    -- Quit.
    if Keyboard.AnyDown(Key.Escape, Key.Q) then
        print("Quitting the game.")
        game_window:close()
    end

    -- Serve action.
    if Keyboard.AnyDown(Key.Space, Key.Enter) then
        print("Spacebar or Enter pressed!")
    end
end

-- Initialize the player when this script is loaded.
Init()

-- Called by the engine each frame.
function update(dt)
    Update(dt)
end
