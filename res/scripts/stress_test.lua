-- physics_stress_test.lua
-- Demonstrates large numbers of simultaneous Box2D bodies.
--
-- Assumptions:
--   * world_id is provided by C++.
--   * Entman.AddCircle(name, world_id, position, radius, color,
--                      isStatic, useMeters) returns an entity.
--   * update(dt) is called once per frame.
--   * Coordinates, radii, and velocities use meters where specified.

local manager = Entman

assert(world_id ~= nil, "world_id was not provided by C++")
assert(manager ~= nil, "Entman is nil")

-- --------------------------------------------------
-- Configuration
-- --------------------------------------------------

local TARGET_COUNT       = 3000
local SPAWN_PER_FRAME    = 40
local SPAWN_INTERVAL     = 0.0 -- Set > 0 to throttle spawning over time.

local SPAWN_CENTER_X     = 15.0
local SPAWN_CENTER_Y     = -2.0
local SPAWN_SPREAD_X     = 5.0

local MIN_RADIUS         = 3
local MAX_RADIUS         = 5

local GROUND_Y           = 15.0
local GROUND_HALF_WIDTH  = 12.0
local GROUND_THICKNESS   = 0.5

local USE_METER_SCALE    = true
local STATIC_BODY        = true
local DYNAMIC_BODY       = false

-- --------------------------------------------------
-- State
-- --------------------------------------------------

local spawned = 0
local spawnAccumulator = 0.0
local elapsed = 0.0
local finished = false

-- --------------------------------------------------
-- Helpers
-- --------------------------------------------------

local function randomColor()
    return ColorRGB.new(
        smath.random_int(60, 255),
        smath.random_int(60, 255),
        smath.random_int(60, 255)
    )
end

local function spawnCircle()
    local x = SPAWN_CENTER_X
        + smath.random_float(-SPAWN_SPREAD_X, SPAWN_SPREAD_X)

    local y = SPAWN_CENTER_Y
        - smath.random_float(0.0, 2.0)

    local radius = smath.random_float(MIN_RADIUS, MAX_RADIUS)

    local entity = manager.AddCircle(
        "StressParticle_" .. tostring(spawned + 1),
        world_id,
        vec2f.new(x, y),
        radius,
        randomColor(),
        DYNAMIC_BODY,
        USE_METER_SCALE
    )

    if entity then
        spawned = spawned + 1
    end
end

local function spawnGround()
    -- A wide, static circle row is not a true flat floor.
    -- Use a static rectangle if your engine exposes AddRect
    -- with the same meter-based coordinate convention.
    manager.AddRect(
        "StressTest_Ground",
        world_id,
        vec2f.new(SPAWN_CENTER_X, GROUND_Y),
        vec2f.new(GROUND_HALF_WIDTH * 2.0, GROUND_THICKNESS),
        ColorRGB.new(90, 100, 115),
        STATIC_BODY,
        USE_METER_SCALE
    )
end

-- --------------------------------------------------
-- Initialization
-- --------------------------------------------------

local function Init()
    spawnGround()
    print("Physics stress test started.")
    print("Target dynamic circles: " .. tostring(TARGET_COUNT))
end

-- --------------------------------------------------
-- Update
-- --------------------------------------------------

local function Update(dt)
    if finished then
        return
    end

    elapsed = elapsed + dt
    spawnAccumulator = spawnAccumulator + dt

    local batch = 0

    if SPAWN_INTERVAL <= 0.0 then
        batch = math.min(SPAWN_PER_FRAME, TARGET_COUNT - spawned)
    else
        while spawnAccumulator >= SPAWN_INTERVAL
            and batch < SPAWN_PER_FRAME
            and spawned + batch < TARGET_COUNT do
            spawnAccumulator = spawnAccumulator - SPAWN_INTERVAL
            batch = batch + 1
        end
    end

    for i = 1, batch do
        spawnCircle()
    end

    if spawned >= TARGET_COUNT then
        finished = true
        print("Stress test spawned " .. tostring(spawned) .. " circles.")
        print("Elapsed spawn time: " .. string.format("%.2f", elapsed) .. " seconds")
    end
end

Init()

function update(dt)
    Update(dt)
end
