#include <af/dim4.hpp>
#include <af/defines.h>
#include <handle.hpp>
#include <err_common.hpp>
#include <backend.hpp>
#include <regions.hpp>

using af::dim4;
using namespace detail;

template<typename T>
static af_array regions(af_array const &in, const int connectivity)
{
    return getHandle<T>(*regions<T>(getArray<uchar>(in), connectivity));
}

af_err af_regions(af_array *out, const af_array in, const int connectivity)
{
    try {
        ARG_ASSERT(2, (connectivity==4 || connectivity==8));

        ArrayInfo info = getInfo(in);
        af::dim4 dims  = info.dims();

        dim_type in_ndims = dims.ndims();
        DIM_ASSERT(1, (in_ndims <= 3 && in_ndims >= 2));

        af_array output;
        af_dtype type  = info.getType();
        switch(type) {
            case f32: output = regions<float >(in, connectivity); break;
            case f64: output = regions<double>(in, connectivity); break;
            case b8 : output = regions<char  >(in, connectivity); break;
            case s32: output = regions<int   >(in, connectivity); break;
            case u32: output = regions<uint  >(in, connectivity); break;
            case u8 : output = regions<uchar >(in, connectivity); break;
            default : TYPE_ERROR(1, type);
        }
        std::swap(*out, output);
    }
    CATCHALL;

    return AF_SUCCESS;
}
