-- pyramid.lua
-- Builds a pyramid of blocks. Then circles fall on it like rain.
-- The Y axis points down: a smaller Y value is higher on the screen.

local world_id = world_id   -- b2WorldId. C++ must set this value.
local manager  = Entman          -- EntityManager usertype.

assert(type(world_id) == "userdata",
    "_world_id must be a b2WorldId value, not " .. type(world_id))
assert(manager ~= nil, "Entman is nil")

-- Settings (all values are in meters)
local CENTER_X      = 15.0   -- X position of the pyramid center
local GROUND_Y      = 16.0   -- Y position of the top of the ground

local BASE_COUNT    = 8      -- number of blocks in the bottom row
local BLOCK_SIZE    = 1.0
local BLOCK_GAP     = 0.02   -- small gap. It prevents overlap at the start.

local RAIN_COUNT    = 1500   -- number of circles
local RAIN_MIN_R    = 2      -- smallest circle radius
local RAIN_MAX_R    = 5      -- largest circle radius
local RAIN_SPREAD   = 9.0    -- circles fall within CENTER_X +/- this value
local RAIN_START_Y  = -5.0   -- height of the first circle (above the screen)
local RAIN_STEP_Y   = 1.5    -- height between circles

-- Body type and scale flags
local STATIC_BODY     = true
local DYNAMIC_BODY    = false   -- the C++ parameter is "isStatic"
local USE_METER_SCALE = true

-- Pyramid: row 0 is the bottom row. Each row has one block less.
local pitch = BLOCK_SIZE + BLOCK_GAP

for row = 0, BASE_COUNT - 1 do
    local count = BASE_COUNT - row
    local t     = row / math.max(BASE_COUNT - 1, 1)   -- 0 at the bottom, 1 at the top

    -- Color changes from orange (bottom) to yellow (top)
    local col = ColorRGB.new(
        math.floor(230 + 25 * t),
        math.floor(110 + 120 * t),
        50)

    local y = GROUND_Y - BLOCK_SIZE * 0.5 - row * pitch

    for c = 0, count - 1 do
        local x = CENTER_X + (c - (count - 1) * 0.5) * pitch
        manager.AddRect(
            "Pyramid Block", world_id,
            vec2f.new(x, y),
            vec2f.new(BLOCK_SIZE, BLOCK_SIZE),
            col, DYNAMIC_BODY, USE_METER_SCALE)
    end
end

-- Rain: circles at different heights, so they reach the pyramid one after the other
for i = 0, RAIN_COUNT - 1 do
    local col = ColorRGB.new(
        smath.random_int(60, 160),
        smath.random_int(120, 255),
        smath.random_int(200, 255))

    manager.AddCircle(
        "Rain Drop", world_id,
        vec2f.new(
            CENTER_X + smath.random_float(-RAIN_SPREAD, RAIN_SPREAD),
            RAIN_START_Y - i * RAIN_STEP_Y),
        smath.random_float(RAIN_MIN_R, RAIN_MAX_R),
        col, DYNAMIC_BODY, USE_METER_SCALE)
end