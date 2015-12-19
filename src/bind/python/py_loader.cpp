#include "py_loader.hpp"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <medusa/loader.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaLoader(void)
{
  bp::class_<Loader, boost::noncopyable>("Loader", bp::no_init)
    .add_property("mame", &Loader::GetName)
    .def("map",           &Loader::Map    )
  ;

  bp::register_ptr_to_python<Loader::SPType>();

  bp::class_<Loader::VSPType>("Loaders")
    .def(bp::vector_indexing_suite<Loader::VSPType, true>())
    ;
}
