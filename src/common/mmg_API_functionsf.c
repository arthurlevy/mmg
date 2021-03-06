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
 * \file common/mmg_API_functionsf.c
 * \brief Fortran API functions for MMG library.
 * \author Algiane Froehly (Inria / IMB, Université de Bordeaux)
 * \version 5
 * \date 01 2014
 * \copyright GNU Lesser General Public License.
 *
 * \note Please, refer to the \ref common/libmmg.h, mmgs/libmmgs.h and
 * mmg3d/libmmg3d.h header files for functions documentation.
 *
 * Define the Fortran API functions for MMG library: adds function
 * definitions with upcase, underscore and double underscore to match
 * any fortran compiler.
 *
 */

#include "mmg.h"


/**
 * See \ref MMG5_Init_fileNames function in \ref common/libmmg.h file.
 */
FORTRAN_NAME(INIT_FILENAMES,init_filenames,(MMG5_pMesh *mesh,MMG5_pSol *sol
#ifdef SINGUL
                                            ,MMG5_pSingul *sing
#endif
               ),(mesh,sol
#ifdef SINGUL
                  ,sing
#endif
                 )) {
#ifdef SINGUL
  MMG5_Init_fileNames(*mesh,*sol,*sing);
#else
  MMG5_Init_fileNames(*mesh,*sol);
#endif
  return;
}

/**
 * See \ref MMG5_Set_inputMeshName function in \ref common/libmmg.h file.
 */
FORTRAN_NAME(MMG5_SET_INPUTMESHNAME, mmg5_set_inputmeshname,
             (MMG5_pMesh *mesh, char* meshin, int *strlen, int* retval),
             (mesh,meshin,strlen,retval)) {
  char *tmp = NULL;

  tmp = (char*)malloc((*strlen+1)*sizeof(char));
  strncpy(tmp,meshin,*strlen);
  tmp[*strlen] = '\0';
  *retval = MMG5_Set_inputMeshName(*mesh,tmp);
  _MMG5_SAFE_FREE(tmp);

  return;
}

/**
 * See \ref MMG5_Set_inputSolName function in \ref common/libmmg.h file.
 */
FORTRAN_NAME(MMG5_SET_INPUTSOLNAME, mmg5_set_inputsolname,
             (MMG5_pMesh *mesh,MMG5_pSol *sol, char* solin, int* strlen, int* retval),
             (mesh,sol,solin,strlen,retval)) {

  char *tmp = NULL;

  tmp = (char*)malloc((*strlen+1)*sizeof(char));
  strncpy(tmp,solin,*strlen);
  tmp[*strlen] = '\0';
  *retval = MMG5_Set_inputSolName(*mesh,*sol,tmp);
  _MMG5_SAFE_FREE(tmp);

  return;
}

/**
 * See \ref MMG5_Set_outputMeshName function in mmgs/libmmgs.h or
 * mmg3d/libmmg3d.h file.
 */
FORTRAN_NAME(MMG5_SET_OUTPUTMESHNAME,mmg5_set_outputmeshname,
             (MMG5_pMesh *mesh, char* meshout, int* strlen,int* retval),
             (mesh,meshout,strlen,retval)){
  char *tmp = NULL;

  tmp = (char*)malloc((*strlen+1)*sizeof(char));
  strncpy(tmp,meshout,*strlen);
  tmp[*strlen] = '\0';
  *retval = MMG5_Set_outputMeshName(*mesh, tmp);
  _MMG5_SAFE_FREE(tmp);

  return;
}

/**
 * See \ref MMG5_Set_outputSolName function in \ref common/libmmg.h file.
 */
FORTRAN_NAME(MMG5_SET_OUTPUTSOLNAME,mmg5_set_outputsolname,
             (MMG5_pMesh *mesh,MMG5_pSol *sol, char* solout,int* strlen, int* retval),
             (mesh,sol,solout,strlen,retval)){
  char *tmp = NULL;

  tmp = (char*)malloc((*strlen+1)*sizeof(char));
  strncpy(tmp,solout,*strlen);
  tmp[*strlen] = '\0';
  *retval = MMG5_Set_outputSolName(*mesh,*sol,tmp);
  _MMG5_SAFE_FREE(tmp);

  return;
}

/**
 * See \ref MMG5_Free_names function in \ref common/libmmg.h file.
 */
FORTRAN_NAME(MMG5_FREE_NAMES,mmg5_free_names,(MMG5_pMesh *mesh,MMG5_pSol *met
#ifdef SINGUL
                                              ,MMG5_pSingul *singul
#endif
               ),(mesh,met
#ifdef SINGUL
                  ,singul
#endif
                 )){
#ifdef SINGUL
  MMG5_Free_names(*mesh,*met,*singul);
#else
  MMG5_Free_names(*mesh,*met);
#endif
  return;
}
