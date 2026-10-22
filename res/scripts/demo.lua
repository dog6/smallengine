-- demo.lua
-- Creates 500 dynamic boxes and circles.

local world_id = game_world_id   -- b2WorldId. C++ must set this value.
local manager  = Entman          -- EntityManager usertype. Use a dot, not a colon.

-- Stop with a clear message if C++ did not set the globals correctly.
assert(type(world_id) == "userdata",
    "game_world_id must be a b2WorldId value, not " .. type(world_id))
assert(manager ~= nil, "Entman is nil")

local DYNAMIC_BODY    = false   -- isStatic = false
local USE_METER_SCALE = true
local USE_PIXEL_SCALE = false

local COUNT = 500

for i = 0, COUNT - 1 do
    local col = ColorRGB.new(
        smath.random_int(60, 255),
        smath.random_int(60, 255),
        smath.random_int(60, 255))

    -- Box with a size in meters
    manager.AddRect(
        "Dynamic Box", world_id,
        vec2f.new(10.0, 50.0 - (i * 2)),
        vec2f.new(1, 1),
        col, DYNAMIC_BODY, USE_METER_SCALE)

    -- Box with a size in pixels
    manager.AddRect(
        "Dynamic Box", world_id,
        vec2f.new(250.0, 50.0 - (i * 12)),
        vec2f.new(10, 10),
        col, DYNAMIC_BODY, USE_PIXEL_SCALE)

    -- Circle with a radius in meters
    manager.AddCircle(
        "Dynamic Circle", world_id,
        vec2f.new(15, 50.0 - (i * 3)),
        5,
        col, DYNAMIC_BODY, USE_METER_SCALE)



end