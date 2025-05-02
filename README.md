# 🎮 C Game – Street Fighter (Ken vs. Ryu)

[![Docs](https://img.shields.io/badge/Doxygen-Documentation-blue?logo=github)](https://shauryapandey.github.io/StreetFighter/index.html)

## About the Project  
> This game was built in C using a barebones OpenGL framework provided by Jeremy Paulding who is the programming professor at Florida Interactive Entertainment Academy. The framework provides a rendering loop, object manager and a level manager. From there, I built as close a clone of the original Street Fighter as possible in 2 weeks. I developed the necessary infrastructure: animation, gameplay logic, collisions, and a behavior-action architecture — everything structured so it could scale.  
---
##🕹️ Gameplay System Design
I leaned into separating gameplay logic from animation logic, building a behavior-action system in order to make the process of creating game content fast and feasible.  
Each character has a `GameplayComponent` and an `AnimationComponent`.
### Actions
Each action (Punch, Kick, Hadouken, etc.) holds:
1. What logic runs at start  
2. What logic runs during timeline  
3. What animation should play and at what rate  
4. If this action can start or interrupt  
5. Cleanup logic when it's interrupted   
This structure made it really easy to extend and plug in new behaviors.
---
##💥 Collisions and Impact Handling
I wanted every object to be capable of collision, but C doesn’t give you inheritance or polymorphism. So I built:
1. A Collider struct
2. A `ColliderVTable` with function pointers:
3. `OnCollisionEnter`
4. `ProcessImpact`  
I realized during the development that I needed to defer the actual processing of impact, since any state change in one would affect how the second collider reacts.
`OnCollisionEnter` only records intent, and `ProcessImpact` applies it later. This avoids messing with state mid-frame.    
### Collision Use Cases
1. Attacks (Melee or ranged)
2. Characters not running into each other
3. Collision points are checked to detect successful blocks or determining type of hurt reaction
4. Characters have `AttackComponent` and `DefenseComponent` to process hits