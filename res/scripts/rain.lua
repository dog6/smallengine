-- rain.lua

local world_id = world_id   -- b2WorldId. C++ must set this value.
local manager  = Entman          -- EntityManager usertype.

assert(type(world_id) == "userdata",
    "_world_id must be a b2WorldId value, not " .. type(world_id))
assert(manager ~= nil, "Entman is nil")

-- Settings (all values are in meters)
local CENTER_X      = 15.0   -- X position of the pyramid center
local RAIN_COUNT    = 2000  -- number of circles
local RAIN_MIN_R    = 3     -- smallest circle radius
local RAIN_MAX_R    = 6      -- largest circle radius
local RAIN_SPREAD   = 15    -- circles fall within CENTER_X +/- this value
local RAIN_START_Y  = -5.0   -- height of the first circle (above the screen)
local RAIN_STEP_Y   = 0.25    -- height between circles

-- Body type and scale flags
local STATIC_BODY     = true
local DYNAMIC_BODY    = false   -- the C++ parameter is "isStatic"
local USE_METER_SCALE = true

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

print("Rain spawned.")
