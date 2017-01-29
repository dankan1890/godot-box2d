#ifndef GODOT_BOX2D_H
#define GODOT_BOX2D_H

#include <object.h>
#include <reference.h>

/***********************************************************************
 * Helpers macros
 **********************************************************************/

#define BOX2D_CLASS(name, parent) \
    protected: class b2##name *entity; Variant metadata; \
    name##B2(class b2##name *); friend class parent; \
    public: ~name##B2(); \
    class b2##name *get_b2() const { return entity; } \
    static name##B2 *get(const b2##name *);

#define BOX2D_PROPERTY(cls, name, type, typename) \
    ObjectTypeDB::bind_method(_MD("get_" #name ":" typename), &cls::get_##name); \
    ObjectTypeDB::bind_method(_MD("set_" #name, "value:" typename), &cls::set_##name); \
    ADD_PROPERTY(PropertyInfo(type, #name), _SCS("set_" #name), _SCS("get_" #name));

#define BOX2D_PROPERTY_ENUM(cls, name, enum) \
    ObjectTypeDB::bind_method(_MD("get_" #name ":" enum), &cls::get_##name); \
    ObjectTypeDB::bind_method(_MD("set_" #name, "value:" enum), &cls::set_##name); \
    ADD_PROPERTY(PropertyInfo(Variant::INT, #name, PROPERTY_HINT_ENUM, #enum), _SCS("set_" #name), _SCS("get_" #name));

#define BOX2D_PROPERTY_BOOL(cls, name) \
    ObjectTypeDB::bind_method(_MD("is_" #name ":bool"), &cls::is_##name); \
    ObjectTypeDB::bind_method(_MD("set_" #name, "value:bool"), &cls::set_##name); \
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, #name), _SCS("set_" #name), _SCS("is_" #name));

#define BOX2D_JOINT(name) \
    private: name##B2(class b2Joint *o) : JointB2(o) {} \
    friend class name##DefB2;

/***********************************************************************
 * Enable conversion from Variant to any class derived from Object
 **********************************************************************/

template<class O>
struct VariantCaster<O*>
{
	static O *cast(const Variant &p_variant)
    {
        auto *obj = (Object*)p_variant;
        return obj ? obj->cast_to<O>() : NULL;
	}
};

/***********************************************************************
 * Math interoperability
 **********************************************************************/

struct b2Vec2 B2(const struct Vector2&);
struct Vector2 GD(const struct b2Vec2&);

struct b2AABB B2(const struct Rect2&);
struct Rect2 GD(const struct b2AABB&);

/***********************************************************************
 * Include entities
 **********************************************************************/

#include "world.h"
#include "body.h"
#include "fixture.h"
#include "shape.h"

#include "joint.h"
#include "revolute_joint.h"
#include "mouse_joint.h"

/***********************************************************************
 * Type Factory
 **********************************************************************/
class Box2D : public Object
{
    OBJ_TYPE(Box2D, Object);
public:
    /** Lifecycle */
    Box2D();
    ~Box2D();

    /** Singleton */
    static Box2D *get() { return singleton; }

    /** Box2D methods */
    WorldB2 *world(const Vector2 &gravity);

    Ref<ShapeB2> circle(const Vector2 &offset, float radius);
    Ref<ShapeB2> box(const Vector2 &extents, const Vector2 &offset = Vector2(), float angle = 0);
    Ref<ShapeB2> poly(const Vector2Array &vertices);
    Ref<ShapeB2> edge(const Vector2 &a, const Vector2 &b);
    Ref<ShapeB2> chain(const Vector2Array &vertices, bool loop = false);

protected:
    /** Godot bindings */
	static void _bind_methods();

    /** Singleton */
    static Box2D *singleton;
};

#endif