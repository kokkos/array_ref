* Make array_view work with a _traits<> class instead of properties. This would
  allow users as well as vendor to extend the behavior of the array.
* Write a paper adding a new type_trait which applies another type_trait to a
  parameter pack and reduces the results with a meta-logical operation (e.g.
  std::conjunction and friends). 
