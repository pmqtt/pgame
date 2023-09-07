## SAT (Separating Axis Theorem) Collision Detection

### Introduction:

Imagine two flat, irregular shapes on a table. If you could slide a thin strip of paper in any direction between these two shapes without it being blocked by the shapes, then they aren't touching. However, if in every direction the strip is blocked, then the two shapes are in contact.

The SAT (Separating Axis Theorem) uses this exact concept but in a mathematical form to check if two shapes are colliding.

### Theory:

1. **Axis Selection**: For every edge of a shape, an "axis" (a straight line) is created perpendicular to it. This means if we have two rectangles, then we have a total of 8 edges, thus 8 axes.

2. **Projection**: Each shape is then projected onto each of these axes. Think of this as the shadow a shape casts when a light source shines on it directly from the side.

3. **Overlap Check**: Now, we look if the shadows (projections) of both shapes on each axis overlap. If they don't overlap on one axis, then the shapes aren't touching.

4. **Collision Detection**: If the shadows overlap on all axes, then the two shapes are colliding.

### Why is this useful?

With SAT, we can not only find out if two shapes are colliding but also how much and in which direction they overlap. This is very handy for computer games and simulations to produce realistic reactions upon collisions.

### Summary:

To check if two shapes are colliding using SAT:

1. Choose axes based on the edges of the shapes.
2. "Cast" shadows of the shapes onto each axis.
3. Check if the shadows overlap on all axes.
4. If they do, the shapes are colliding!

