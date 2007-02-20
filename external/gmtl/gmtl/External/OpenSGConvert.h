#ifndef GMTL_OPENSG_CONVERT_H_
#define GMTL_OPENSG_CONVERT_H_

/** @file OpenSGConvert.h GMTL to OpenSG conversion functions
* 
* methods to convert between gtml and opensg matrix classes
*/

#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <OpenSG/OSGMatrix.h>

namespace gmtl
{

/** Convert an opensg matrix to a gmtl::Matrix
 * @param mat        the matrix to write the OpenSG matrix data into
 * @param osg_mat    the source OpenSG matrix
 * @return     returns the equivalent GMTL matrix
 */
inline
Matrix44f& set( Matrix44f& mat, const osg::Matrix& osg_mat )
{
   mat.set(osg_mat.getValues());
   return mat;
}

/** Convert a GMTL matrix to a OpenSG matrix
 * @param osg_mat     the matrix to write the GMTL matrix data into
 * @param mat         the source GMTL matrix
 * @return     returns the equivalent OpenSG matrix
 */
inline
osg::Matrix& set(osg::Matrix& osg_mat, const Matrix44f& mat)
{
   osg_mat.setValue( mat.getData() );
   return osg_mat;
}

}

#endif

