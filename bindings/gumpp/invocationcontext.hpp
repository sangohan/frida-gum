#include "gumpp.hpp"

#include <gum/gum.h>

namespace Gum
{
  class InvocationContextImpl : public InvocationContext
  {
  public:
    InvocationContextImpl (GumInvocationContext * ctx)
      : context (ctx),
        parent (NULL)
    {
    }

    ~InvocationContextImpl ()
    {
      delete parent;
    }

    virtual void * get_nth_argument (unsigned int n)
    {
      return gum_invocation_context_get_nth_argument (context, n);
    }

    virtual void replace_nth_argument (unsigned int n, void * value)
    {
      gum_invocation_context_replace_nth_argument (context, n, value);
    }

    virtual void * get_return_value ()
    {
      return gum_invocation_context_get_return_value (context);
    }

    virtual InvocationContext * get_parent ()
    {
      if (parent == NULL)
      {
        GumInvocationContext * parent_context = gum_invocation_context_get_parent (context);
        if (parent_context == NULL)
          return NULL;
        parent = new InvocationContextImpl (parent_context);
      }

      return parent;
    }

  private:
    GumInvocationContext * context;
    InvocationContextImpl * parent;
  };
}