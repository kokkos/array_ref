==============================================================================
Issues with **array_view** (P0122) resolved by **view** (P0009)
==============================================================================



******************************************************************************
Multidimensional Declaration and Access
******************************************************************************

The **array_view** API requires the magic number **dynamic_length** to specify implicit dimensions.  In contrast the base **view** proposal uses **[]** to denote an implicit dimension, and resorts to **0** for the magic value for compatibility with **std::extent**.

The **array_view** range check is always enabled which necessarily
incurs an overhead - a showblocker for numerical users.  In contrast the **view** proposal bounds check
is optional and selectable on a per-view basis.  Even with static extents,
the bounds check incurs overhead unless the index itself is **constexpr**
(which we suspect this implementation can support as an optimization).

The **array_view** design requires numerous public helper classes
for multidimensional array declarations and access.
In contrast the base **view** proposal does not require
any public helper classes.  This base proposal does require
a trivial change to permit flexible array type declarations.

The element access operator() for **view** is simple and
intuitive across multiple mathematically-oriented languages,
and has been in use for over five decades.
Multidimensional arrays are the bedrock of numerical computations
in mathematics, science, and engineering.  A multidimensional array
capability must be appealing to these communities.  Furthermore, how can
slices of the type **myarray(1, std::all, 4)**, an essential feture,
be easily supported and optimized with a **myarray[{1, std::all, 4}]** index
without overhead or an extremely complicated index class?

Element access operators for **array_view** are highly unlikely
to result in optimal element access code generation,
unless optimizations for these specific patterns are incorporated
into compilers.
In contrast the element access operators for **view** have resulted
in optimized instruction generation and even allowed vectorizaton without
any special recognition or treatment by a compiler.

The array_view proposal for **strided_array_view** is wholely
inadequate to optimally express and implement the variety of
multidimensional layouts that are required for portable and
performant numerical computations.
Note that the **strided_array_view** is incompatible with
the array_view template arguments,
Note that **strided_array_view** cannot accept explicit dimensions
and as such cannot have optimized member access for explicit dimensions
and is not vectorizable with all dimensions requiring strides.

Moreover, the incomplete **strided_array_view** has no support for static extents
and strides, and makes slices of statically sized **array_view** almost useless.
All of the useful static information kept around for compiler optimizations is then
thrown away.

In summary **array_view** is a rank-one focused proposal that
for practical purposes has multdimensional capability as an
awkward add-on.  The **view** proposal is a multidimensional array 
proposal targeting numerical computations.

They suggest using a strided_array_view to represent column-major data.
(1) As noted above, the strided view is butchered due to its lack of explicit dimensions, and couldn't possibly generate optimal assembler code for element access;
(2) Column-major data is fully contiguous and can rely on all contiguous optimizations; and
(3) It is essential that the storage-order can be checked staticically to dispatch to the appropriate algorithm (e.g., with 2 dimensions, you change whether you iterate through rows or columns first depending on the storage order).

The **array_view** is written to have support for ranges.  There seems to be nothing to prevent the same patterns to be used with the **view** class.

******************************************************************************
Extensibility
******************************************************************************

The **view** proposal is intended to be extensible for other
mechanisms by which a span of memory is viewed.

  - The atomic view proposal P0019 could be subsumed into the **view** proposal with an *atomic* member access property.
  - Contemporary and future computing architectures with heterogeneous memory systems can denote the memory space of a **view** with a memory space property.
  - Specialized properties such as non-aliasing (restrict), non-temporal (streaming), or random access can be leveraged by an implementation.  For example *const random access* on a GPU architecture can leverage texture cache.


