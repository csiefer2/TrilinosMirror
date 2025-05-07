 cmake  \
            -DTrilinos_ENABLE_ALL_FORWARD_DEP_PACKAGES=OFF \
            -DTrilinos_ENABLE_ALL_OPTIONAL_PACKAGES=OFF \
            -DTrilinos_ENABLE_SECONDARY_TESTED_CODE=OFF \
            -DTrilinos_ENABLE_Amesos=OFF \
            -DTrilinos_ENABLE_AztecOO=OFF \
            -DTrilinos_ENABLE_Epetra=OFF \
            -DTrilinos_ENABLE_EpetraExt=OFF \
            -DTrilinos_ENABLE_Ifpack=OFF \
            -DTrilinos_ENABLE_Intrepid=OFF \
            -DTrilinos_ENABLE_Isorropia=OFF \
            -DTrilinos_ENABLE_ML=OFF \
            -DTrilinos_ENABLE_NewPackage=OFF \
            -DTrilinos_ENABLE_Pliris=OFF \
            -DTrilinos_ENABLE_PyTrilinos=OFF \
            -DTrilinos_ENABLE_ShyLU_DDCore=OFF \
            -DTrilinos_ENABLE_ThyraEpetraAdapters=OFF \
            -DTrilinos_ENABLE_ThyraEpetraExtAdapters=OFF \
            -DLINK_FLAGS="$LIB_FUZZING_ENGINE" \
            -DTrilinos_ENABLE_Triutils=OFF \
            ..
            
          make -j 16
