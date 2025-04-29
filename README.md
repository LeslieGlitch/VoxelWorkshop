# Voxel Workshop
A project by Leslie Wollitz  

---

Voxel Workshop is a physics and rendering engine designed to work with voxelized
objects. The goal for the project is to allow users to create objects and interact with them in a physics simulation in real time.

---
## Video Demo

https://github.com/user-attachments/assets/b5d359e0-3a89-4a08-9c1d-18e8175aafff

---

## Overall Structure
The engine follows a classic event-driven game loop model in three phases:  
1. At launch, there is an initialization phase where the window is opened and an
empty scene is loaded.
1. Next, the program begins a render loop, where the GUI is drawn, and any objects
in the scene are updated and rendered.
1. Once the user exits the program (via `Esc`), the program cleans up resources and exits.

There is also a class hierarchy for representing the state of the scene.
- At the top is the Scene, which handles communication between the user and the
objects in a scene. The Scene also handles distributing function calls to all objects.
- Objects are the structures that exist within the scene. They keep track of their
position, velocity, and acceleration along with hosting several functions related
to updating their physics.
    - There are two distinct types of Object, being Rigid Bodies and Static Bodies:
        - Rigid Bodies are the typical object that can move around and be affected
        by physics
        - Static Bodies are special objects that stay still and are unaffected by
        collisions or gravity
- Brickmaps are the data structure that hold the shape of an object. In the current
implementation, these are limited to 8\*8\*8 voxels. The Brickmap data structure
was invented by Thijs van Wingerden in their paper
[Real-time Ray tracing and Editing of Large Voxel Scenes](https://studenttheses.uu.nl/bitstream/handle/20.500.12932/20460/final.pdf?sequence=2&isAllowed=y).
While the paper goes in-depth about
the data structure and its applications, Voxel Workshop only implements a limited
version of the base Brickmap structure with a bitmask.

---

## Implementation Details

### Rotation
Typically, rotating a model in 3D space is handled by multiplying the coordinates
of each vertex on the model by a rotation matrix. The rendering engine for Voxel
Workshop uses this method for displaying objects to the user. However, this runs
into some issues in the case of rotating voxelized structures for physics
calculations. Voxelized objects must align to a world grid, and each voxel can
only hold one data point. Using typical rotation methods, each voxel in the world
grid would find the nearest voxel in the rotated structure and use that. This can
sometimes lead to the number of voxels in the structure changing. To combat this, I
implemented rotation using a series of skews in a method invented by Andrew Taylors
and described on his
[website](https://github.andrewt.net/shears/).
This method of rotation ensures that each voxel in the input corresponds to exactly
one voxel in the output. This helps to ensure accuracy with the rotated structure
and the size/mass of the corresponding object.

### Collision
In a mesh-based physics engine, there are two forms of collision based on the
shape of the mesh. The edge of a mesh can collide with another mesh's edge, or a
vertex can collide with a face. In a voxel engine, however, there is a simpler
solution that I implemented with a four-step process to check for collision
between an object and a potential collider.  
1. The object performs a distance test to see if the collider is close enough
to potentially collide with the object.
1. If the collider is close enough, the object generates a rotated Brickmap
structure for itself and the collider.
1. The collider bitmask is offset by the physical offset between objects, and
an AND operation is used to check for overlap.
1. If an overlap exists, there is a collision. The centroid of the collision
region is calculated, and an impulse force is applied.

### Impulse Forces
Forces applied to objects in Voxel Workshop are physically-based, and follow
real-world physics equations. This involves a linear impulse that affects the
object's position, and a rotational impulse that affects the object's rotation.
- Linear
    - The linear portion begins with Newton's Second Law: $F = ma$.
    - Rearranging, we get $a = F/m$.
    - Impulses are forces applied for the duration of a time period, so we get
    $a = {Impulse}/({m*\delta})$, where $\delta$ is the length of time a frame takes.
    - This computed acceleration value can be added to the object's acceleration,
    which will propagate down to the object's position.
- Rotational
    - The rotational portion is calculated using the offset distance and direction
    of the impulse force. We start here with $\tau = d*F$, where $\tau$ is the
    torque and $d$ is the distance from the object's center of mass.
    - Of note, the force applied here is only the force component pointing
    perpendicular to the impulse force's offset from the center of mass.
    - We once again use Impulse instead of force, giving us a final equation
    of $\tau = d * F * (\vec{offset} \cdot \vec{Force Direction})/\delta$.
    - We can use this to update the object's rotational acceleration, which
    will propagate down to the object's rotation.

---

## Future Work
Voxel Workshop is far from complete, and has a lot of work left to go which
will likely take years more development time. The work left to be done includes:
- Fully implementing the Brickmap structure with Brickgrids and chunking to
allow for larger objects
- Implement an object management class to allow for more objects per scene and
object unloading
- Optimize performance
- Implement a lighting and sound engine
- Create a Brickmap structure editor so users can define custom shapes
- Add more object interaction forces, such as friction, temperature,
conductivity, and more
- Overhaul the GUI to be more user-friendly
- Give better inputs for object parameters, especially in the case of rotation,
which needs to be abstracted away from the quaternions it uses internally
- Allow users to create custom scripts and attach them to an object, giving
custom interaction.
