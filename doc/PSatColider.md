Sure, here's a `README.md` for the `PSatColider` class:

---

# PSatColider Class Documentation

`PSatColider` is a class that extends the `PColider` base class. It implements the Separating Axis Theorem (SAT) for 2D shape collision detection. 

## Public Methods

### `void prin_point_2d(const PPoint2D &p) const`

Prints a given 2D point in the format `(x, y)`.

**Parameters**:
- `p`: The 2D point to be printed.

### `auto colide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool`

Determines if two shapes collide based on their bounding boxes.

**Parameters**:
- `p1`: A shared pointer to the first drawable shape.
- `p2`: A shared pointer to the second drawable shape.

**Returns**: 
- `true` if the two shapes collide, `false` otherwise.

### `auto overlapAmount(const PPoint2D& axis, const PVertices2D& shape1, const PVertices2D& shape2) const -> float`

Calculates the overlap amount between two shapes along a given axis.

**Parameters**:
- `axis`: The axis along which the overlap is to be determined.
- `shape1`: The vertices of the first shape.
- `shape2`: The vertices of the second shape.

**Returns**: 
- The amount of overlap between the two shapes along the given axis.

### `auto normals()const -> std::array<float, 2>`

Returns the collision normal which indicates the direction and magnitude of the overlap.

**Returns**: 
- An array of two floats representing the collision normal.

## Private Members

### `mutable std::array<float, 2> _collision_normal`

Stores the collision normal. Initialized to `{0.0f, 0.0f}`.

