# 🎮 C Game – Street Fighter (Ken vs. Ryu)

![Gameplay Screenshot](https://i.imgur.com/ltCjQKk.png)

## 📚 Documentation
For detailed description of the classes and the relationship diagrams, check this out:  

[![Docs](https://img.shields.io/badge/Doxygen-Documentation-blue?logo=github)](https://shauryapandey.github.io/StreetFighter/index.html)

---
## About the Project 
This game was built in C using a barebones OpenGL framework provided by Jeremy Paulding who is the programming professor at Florida Interactive Entertainment Academy. The framework provides a rendering loop, object manager and a level manager. From there, I built as close a clone of the original Street Fighter as possible in 2 weeks. I developed the necessary infrastructure: animation, gameplay logic, collisions, and a behavior-action architecture — everything structured so it could scale.
If you want to play this game on your PC, then go ahead and clone the repo, then you will need visual studio to build it. I developed using VS2022 but I think earlier versions should also be able to compile the game, you may need to get the extensions for OpenGL.

---
### The controls are:
For player 1:
W,A,S,D for moving, jumping and crouching.
F and G for punching and kicking.
You can perform jump/crouch attacks as well by pressing the right combination of keys.
1 to Block.
S+A/D+F to do the Hadouken energy blast.

---
For Player 2:
Arrow keys for moving, jumping and crouching.
M and N for punching and kicking.
9 to block.
Right/Left + Down + M for Hadouken energy blast.

---
## 🕹️ Gameplay System Design
I leaned into separating gameplay logic from animation logic, building a behavior-action system in order to make the process of creating game content fast and feasible.  
Each character has a `GameplayComponent` and an `AnimationComponent`. dsaf
### Actions
Each action (Punch, Kick, Hadouken, etc.) holds:
- What logic runs at start
- What logic runs during timeline
- What animation should play and at what rate
- What logic to decide if this action can start
- Cleanup logic when it's interrupted
- Priority which is used in processing interruptions   
This structure made it really conducive to extend and plug in new behaviors.
---
## 💥 Collisions and Impact Handling
I wanted every object to be capable of collision, but C doesn’t give you inheritance or polymorphism. So I built:
- A `Collider` struct
- A `ColliderVTable` with function pointers:
    - `OnCollisionEnter`
    - `ProcessImpact`
 
I realized during the development that I needed to defer the actual processing of impact, since any state change in one would affect how the second collider reacts.
`OnCollisionEnter` only records intent, and `ProcessImpact` applies it later. This avoids messing with state mid-frame.    
### Collision Use Cases
- Attacks (Melee or ranged)
- Characters not running into each other
- Collision points are checked to detect successful blocks or determining type of hurt reaction
- Characters have `AttackComponent` and `DefenseComponent` to process hits
---
## 🎞️ Animation System
Built a Sprite Animation system from scratch. Each `Animation` has a list of Frames and durations.
- Action owns animation
- Frame rate controlled based on gameplay timeline
- Used free sprite sheets and a cutout tool to define animation sequences
- Character-specific animation init functions (e.g. `InitRyuAnimations`) called in `LevelManager`
---
## 📦 Object Management
The core logic was handled in the framework that I inherited. An `ObjectManager` that stores and updates all objects and a `LevelManager` that sets up the level.
I extended the code in the `LevelManager` to also initialize the textures, and sounds making use of function pointers to provide function callbacks to the various objects in the level. I also handle the memory leak free loading and unloading.

---
## 👩‍👦 Polymorphism in C
Since C does not have classes like C++,I mimicked polymorphism using:
- VTable structs
- Function pointers in each object's VTable.
- Each subclass (Hadouken, Character) implements its behavior through the vtable.
```c
typedef struct object_vtable_t {
    ObjDrawFunc     draw;
    ObjUpdateFunc   update;
} ObjVtable;

typedef struct object_t {
    ObjVtable*      vtable;
    Collider*       collider;
    ObjectType      type;
    Bounds2D        levelDef;
    Coord2D         position;
    Coord2D         velocity;
    Coord2D         size;
} Object;
```
## 🧠 Key Learnings
- Thinking in systems: Decoupling animations from gameplay
- Every constraint was an opportunity to get creative
