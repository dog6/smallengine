-- Player movement settings
local PLAYER_SPEED = 300
local RECT_SIZE = vec2f(50, 50)

local myRectangle = nil

function Init()
    -- Create a dynamic player using pixel-based coordinates.
    myRectangle = Entman.AddRect(
        "Player1",
        world_id,
        vec2f(400, 300),
        RECT_SIZE,
        ColorRGB(255, 0, 0),
        false, -- Dynamic body
        false  -- Pixel-based creation coordinates
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

    -- Read continuous movement input.
    local moveX = Keyboard.Axis(Key.A, Key.D, 1)
    local moveY = Keyboard.Axis(Key.W, Key.S, 1)

    -- Normalize diagonal movement.
    if moveX ~= 0 and moveY ~= 0 then
        local diagonalScale = 0.70710678
        moveX = moveX * diagonalScale
        moveY = moveY * diagonalScale
    end

    -- Apply velocity in pixels per second.
    myRectangle:SetLinearVelocity(
        vec2f(
            moveX * PLAYER_SPEED,
            moveY * PLAYER_SPEED
        )
    )

    -- Quit only once per key press.
    if Keyboard.IsPressed(Key.Escape)
        or Keyboard.IsPressed(Key.Q) then
        print("Quitting the game.")
        game_window:close()
    end

    -- Trigger the serve action only once per key press.
    if Keyboard.IsPressed(Key.Space)
        or Keyboard.IsPressed(Key.Enter) then
        print("Spacebar or Enter pressed!")
    end
end

-- Initialize the player when this script is loaded.
Init()

-- Called by the engine each frame.
function update(dt)
    Update(dt)
end