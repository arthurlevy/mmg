/* =============================================================================
**  This file is part of the mmg software package for the tetrahedral
**  mesh modification.
**  Copyright (c) Inria - IMB (Université de Bordeaux) - LJLL (UPMC), 2004- .
**
**  mmg is free software: you can redistribute it and/or modify it
**  under the terms of the GNU Lesser General Public License as published
**  by the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  mmg is distributed in the hope that it will be useful, but WITHOUT
**  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
**  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
**  License for more details.
**
**  You should have received a copy of the GNU Lesser General Public
**  License and of the GNU General Public License along with mmg (in
**  files COPYING.LESSER and COPYING). If not, see
**  <http://www.gnu.org/licenses/>. Please read their terms carefully and
**  use this copy of the mmg distribution only if you accept them.
** =============================================================================
*/

/**
 * \file common/mmg_anisosiz.c
 * \brief Fonctions for anisotropic size map computation.
 * \author Charles Dapogny (LJLL, UPMC)
 * \author Cécile Dobrzynski (Inria / IMB, Université de Bordeaux)
 * \author Pascal Frey (LJLL, UPMC)
 * \author Algiane Froehly (Inria / IMB, Université de Bordeaux)
 * \version 5
 * \copyright GNU Lesser General Public License.
 */

#include "mmgs.h"

/**
 * \param mesh pointer toward the mesh structure.
 * \param met pointer toward the sol structure.
 * \param np0 index of edge's extremity.
 * \param np1 index of edge's extremity.
 * \param isedg 1 if the edge is a ridge, 0 otherwise.
 * \return length of edge according to the prescribed metric.
 *
 * Compute length of edge \f$[i0;i1]\f$ according to the prescribed aniso.
 * metric.
 *
 */
double _MMG5_lenedg_ani(MMG5_pMesh mesh,MMG5_pSol met,int np0,int np1,char isedg) {
  MMG5_pPoint   p0,p1;
  double        gammaprim0[3],gammaprim1[3],t[3],*n1,*n2,ux,uy,uz,ps1,ps2,l0,l1;
  double        *m0,*m1,met0[6],met1[6];

  p0 = &mesh->point[np0];
  p1 = &mesh->point[np1];

  ux = p1->c[0] - p0->c[0];
  uy = p1->c[1] - p0->c[1];
  uz = p1->c[2] - p0->c[2];

  /* computation of the two tangent vectors to the underlying curve of [i0i1] */
  if ( MS_SIN(p0->tag) ) {
    gammaprim0[0] = ux;
    gammaprim0[1] = uy;
    gammaprim0[2] = uz;
  }
  else if ( isedg ) {
    memcpy(t,p0->n,3*sizeof(double));
    ps1 = ux*t[0] + uy*t[1] + uz*t[2];
    gammaprim0[0] = ps1*t[0];
    gammaprim0[1] = ps1*t[1];
    gammaprim0[2] = ps1*t[2];
  }
  else {
    if ( MG_GEO & p0->tag ) {
      //assert(p0->ig);
      n1 = &mesh->xpoint[p0->ig].n1[0];
      n2 = &mesh->xpoint[p0->ig].n2[0];
      ps1 = ux*n1[0] + uy*n1[1] + uz*n1[2];
      ps2 = ux*n2[0] + uy*n2[1] + uz*n2[2];

      if ( fabs(ps2) < fabs(ps1) ) {
        n1  = &mesh->xpoint[p0->ig].n2[0];
        ps1 = ps2;
      }
    }
    else if ( MG_REF & p0->tag ) {
      n1  = &mesh->xpoint[p0->ig].n1[0];
      ps1 = ux*n1[0] + uy*n1[1] + uz*n1[2];
    }
    else {
      n1  = &(p0->n[0]);
      ps1 = ux*n1[0] + uy*n1[1] + uz*n1[2];
    }
    gammaprim0[0] = ux - ps1*n1[0];
    gammaprim0[1] = uy - ps1*n1[1];
    gammaprim0[2] = uz - ps1*n1[2];
  }

  if ( MS_SIN(p1->tag) ) {
    gammaprim1[0] = -ux;
    gammaprim1[1] = -uy;
    gammaprim1[2] = -uz;
  }
  else if ( isedg ) {
    memcpy(t,p1->n,3*sizeof(double));
    ps1 = -ux*t[0] - uy*t[1] - uz*t[2];
    gammaprim1[0] = ps1*t[0];
    gammaprim1[1] = ps1*t[1];
    gammaprim1[2] = ps1*t[2];
  }
  else {
    if ( MG_GEO & p1->tag ) {
      n1 = &mesh->xpoint[p1->ig].n1[0];
      n2 = &mesh->xpoint[p1->ig].n2[0];
      ps1 = -ux*n1[0] - uy*n1[1] - uz*n1[2];
      ps2 = -ux*n2[0] - uy*n2[1] - uz*n2[2];

      if ( fabs(ps2) < fabs(ps1) ) {
        n1  = &mesh->xpoint[p1->ig].n2[0];
        ps1 = ps2;
      }
    }
    else if ( MG_REF & p1->tag ) {
      n1  = &mesh->xpoint[p1->ig].n1[0];
      ps1 = - ux*n1[0] - uy*n1[1] - uz*n1[2];
    }
    else {
      n1  = &(p1->n[0]);
      ps1 = -ux*n1[0] - uy*n1[1] - uz*n1[2];
    }
    gammaprim1[0] = - ux - ps1*n1[0];
    gammaprim1[1] = - uy - ps1*n1[1];
    gammaprim1[2] = - uz - ps1*n1[2];
  }

  /* Set metrics */
  if ( MS_SIN(p0->tag) ) {
    m0 = &met->m[6*(np0)+1];
  }
  else if ( MG_GEO & p0->tag ) {
    if ( !_MMG5_buildridmet(mesh,met,np0,ux,uy,uz,met0) )  return(-1.0);
    m0 = met0;
  }
  else {
    m0 = &met->m[6*(np0)+1];
  }

  if ( MS_SIN(p1->tag) ) {
    m1 = &met->m[6*(np1)+1];
  }
  else if ( MG_GEO & p1->tag ) {
    if ( !_MMG5_buildridmet(mesh,met,np1,ux,uy,uz,met1) )  return(-1.0);
    m1 = met1;
  }
  else {
    m1 = &met->m[6*(np1)+1];
  }

  /* computation of the length of the two tangent vectors in their respective tangent plane */
  l0 = m0[0]*gammaprim0[0]*gammaprim0[0] + m0[3]*gammaprim0[1]*gammaprim0[1] \
    + m0[5]*gammaprim0[2]*gammaprim0[2] \
    + 2.0*m0[1]*gammaprim0[0]*gammaprim0[1]  + 2.0*m0[2]*gammaprim0[0]*gammaprim0[2] \
    + 2.0*m0[4]*gammaprim0[1]*gammaprim0[2];

  l1 = m1[0]*gammaprim1[0]*gammaprim1[0] + m1[3]*gammaprim1[1]*gammaprim1[1] \
    + m1[5]*gammaprim1[2]*gammaprim1[2] \
    +2.0*m1[1]*gammaprim1[0]*gammaprim1[1]  + 2.0*m1[2]*gammaprim1[0]*gammaprim1[2] \
    + 2.0*m1[4]*gammaprim1[1]*gammaprim1[2];

  if(l0 < 0) {
    printf("neg %e\n",l0);
    l0 =1;
  }
  if(l1 < 0) {
    printf("neg1 %e\n",l1);
    l1 = 1;
  }
  l0 = 0.5*(sqrt(l0) + sqrt(l1));
  return(l0);
}

/**
 * \param mesh pointer toward the mesh structure.
 * \param met pointer toward the meric structure.
 * \param ptt pointer toward the triangle structure.
 * \return The computed area.
 *
 * Compute the area of the surface triangle \a ptt with respect to
 * the anisotropic metric \a met.
 *
 */
double _MMG5_surftri_ani(MMG5_pMesh mesh,MMG5_pSol met,MMG5_pTria ptt) {
  MMG5_pPoint    p[3];
  _MMG5_Bezier    b;
  int       np[3];
  double    surf,ux,uy,uz,dens,m[3][6],J[3][2],mJ[3][2],tJmJ[2][2];
  char      i,i1,i2;

  surf = 0.0;

  for (i=0; i<3; i++) {
    np[i] = ptt->v[i];
    p[i]  = &mesh->point[np[i]];
  }
  if ( !_MMG5_bezierCP(mesh,ptt,&b,1) ) return(0.0);

  /* Set metric tensors at vertices of tria iel */
  for(i=0; i<3; i++) {
    i1 = _MMG5_inxt2[i];
    i2 = _MMG5_iprv2[i];
    ux = 0.5*(p[i1]->c[0]+p[i2]->c[0]) - p[i]->c[0];
    uy = 0.5*(p[i1]->c[1]+p[i2]->c[1]) - p[i]->c[1];
    uz = 0.5*(p[i1]->c[2]+p[i2]->c[2]) - p[i]->c[2];

    if ( MS_SIN(p[i]->tag) ) {
      memcpy(&m[i][0],&met->m[6*np[i]+1],6*sizeof(double));
    }
    else if ( p[i]->tag & MG_GEO ) {
      if ( !_MMG5_buildridmet(mesh,met,np[i],ux,uy,uz,&m[i][0]) )  return(0.0);
    }
    else {
      memcpy(&m[i][0],&met->m[6*np[i]+1],6*sizeof(double));
    }
  }

  /* Compute density integrand of volume at the 3 vertices of T */
  for (i=0; i<3; i++) {
    if ( i == 0 ) {
      J[0][0] = 3.0*( b.b[7][0] - b.b[0][0] ) ; J[0][1] = 3.0*( b.b[6][0] - b.b[0][0] );
      J[1][0] = 3.0*( b.b[7][1] - b.b[0][1] ) ; J[1][1] = 3.0*( b.b[6][1] - b.b[0][1] );
      J[2][0] = 3.0*( b.b[7][2] - b.b[0][2] ) ; J[2][1] = 3.0*( b.b[6][2] - b.b[0][2] );
    }
    else if ( i == 1 ) {
      J[0][0] = 3.0*( b.b[1][0] - b.b[8][0] ) ; J[0][1] = 3.0*( b.b[3][0] - b.b[8][0] );
      J[1][0] = 3.0*( b.b[1][1] - b.b[8][1] ) ; J[1][1] = 3.0*( b.b[3][1] - b.b[8][1] );
      J[2][0] = 3.0*( b.b[1][2] - b.b[8][2] ) ; J[2][1] = 3.0*( b.b[3][2] - b.b[8][2] );
    }
    else {
      J[0][0] = 3.0*( b.b[4][0] - b.b[5][0] ) ; J[0][1] = 3.0*( b.b[2][0] - b.b[5][0] );
      J[1][0] = 3.0*( b.b[4][1] - b.b[5][1] ) ; J[1][1] = 3.0*( b.b[2][1] - b.b[5][1] );
      J[2][0] = 3.0*( b.b[4][2] - b.b[5][2] ) ; J[2][1] = 3.0*( b.b[2][2] - b.b[5][2] );
    }

    mJ[0][0] = m[i][0]*J[0][0] + m[i][1]*J[1][0] + m[i][2]*J[2][0];
    mJ[1][0] = m[i][1]*J[0][0] + m[i][3]*J[1][0] + m[i][4]*J[2][0];
    mJ[2][0] = m[i][2]*J[0][0] + m[i][4]*J[1][0] + m[i][5]*J[2][0];

    mJ[0][1] = m[i][0]*J[0][1] + m[i][1]*J[1][1] + m[i][2]*J[2][1];
    mJ[1][1] = m[i][1]*J[0][1] + m[i][3]*J[1][1] + m[i][4]*J[2][1];
    mJ[2][1] = m[i][2]*J[0][1] + m[i][4]*J[1][1] + m[i][5]*J[2][1];

    /* dens = sqrt(tJacsigma * M * Jacsigma )*/
    tJmJ[0][0] = J[0][0]*mJ[0][0] + J[1][0]*mJ[1][0] + J[2][0]*mJ[2][0];
    tJmJ[0][1] = J[0][0]*mJ[0][1] + J[1][0]*mJ[1][1] + J[2][0]*mJ[2][1];
    tJmJ[1][0] = J[0][1]*mJ[0][0] + J[1][1]*mJ[1][0] + J[2][1]*mJ[2][0];
    tJmJ[1][1] = J[0][1]*mJ[0][1] + J[1][1]*mJ[1][1] + J[2][1]*mJ[2][1];

    dens = tJmJ[0][0]*tJmJ[1][1] - tJmJ[1][0]*tJmJ[0][1];
    if ( dens < 0.0 ) {
      //fprintf(stdout,"  ## Density should be positive : %E for elt %d %d %d \n",dens,ptt->v[0],ptt->v[1],ptt->v[2]);
      //return(0.0);
    }
    surf += sqrt(fabs(dens));
  }

  surf *= _MMG5_ATHIRD;
  return(surf);
}
