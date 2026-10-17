#include "core/entity.hpp"

using namespace sg::Core;

Entity::Entity()
{
    this->drawable = nullptr;
}

Entity::~Entity()
{
    if (this->drawable != nullptr) free(this->drawable);
}



void Entity::SetDrawable(sf::Drawable &_drawable) { this->drawable = &_drawable; }

sf::Drawable &Entity::GetDrawable() { return *this->drawable; }