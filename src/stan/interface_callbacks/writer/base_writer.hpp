#ifndef STAN_INTERFACE_CALLBACKS_WRITER_BASE_WRITER_HPP
#define STAN_INTERFACE_CALLBACKS_WRITER_BASE_WRITER_HPP

#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

namespace stan {
  namespace interface_callbacks {
    namespace writer {
      
      // base_writer is an absract base class defining the
      // interface for Stan writer callbacks.  The Stan API
      // writes structured output to implementations of this
      // class defined by a given interface.
      //
      // A typical API function signature will be of the form
      //
      // template <WriterA, WriterB>
      // void function(WriterA& writer_a, WriterB& writer_b) {
      //   ...
      //   writer_a(key, value);
      //   writer_b("message");
      //   ...
      // }
      //
      // where WriterA and WriterB are base_writer implementations.
      
      class base_writer {
      public:
        // Virtual destructor to avoid compiler warnings
        virtual ~base_writer() {};
        
        // Key-scalar input
        virtual void operator()(const std::string& key,
                                double value) = 0;
        virtual void operator()(const std::string& key,
                                const std::string& value) = 0;
        
        // Key-array input, used for contingous Eigen vectors
        virtual void operator()(const std::string& key,
                                const double* values,
                                int n_values) = 0;
        
        // Key-array input, used for contiguous Eigen matrices
        virtual void operator()(const std::string& key,
                                const double* values,
                                int n_rows, int n_cols) = 0;
        
        // STL vectors input, used for state names and values
        virtual void operator()(const std::vector<std::string>& names) = 0;
        virtual void operator()(const std::vector<double>& state) = 0;
        
        // Flush operation
        virtual void operator()() = 0;
        
        // String input
        virtual void operator()(const std::string& message) = 0;
        
      };

    }
  }
}

#endif
