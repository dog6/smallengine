#include "physics/box2dworld.hpp"

// Make the pool first. Keep it alive for as long as the world exists.
sg::TaskPool pool(4);   // 4 workers, including the calling thread

sg::Physics::Box2DWorld::Box2DWorld(b2Vec2 _gravity, sg::TaskPool* pool)
{
    this->worldDef = b2DefaultWorldDef();
    this->worldDef.gravity = _gravity;
    this->worldDef.workerCount = pool->WorkerCount();
    this->worldDef.enqueueTask = &sg::TaskPool::Enqueue;
    this->worldDef.finishTask = &sg::TaskPool::Finish;
    this->worldDef.userTaskContext = pool;
    this->worldId = b2CreateWorld(&worldDef);
}