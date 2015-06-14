BUILD_DIR = build

$(BUILD_DIR_):
	-mkdir $(BUILD_DIR_) 2>/dev/null
	-cd $(BUILD_DIR); rm -rf *

# The release target will perform additional optimization
.PHONY : release
release: $(BUILD_DIR)
	cd $(BUILD_DIR); \
	cmake -DCMAKE_BUILD_TYPE=Release ..; \
	cp build/README.md .

# Enables CuTest unit testing
.PHONY : debug
debug: $(BUILD_DIR)
	cd $(BUILD_DIR); \
	cmake -DTEST=1 ..; \
	cp build/README.md .

# For Mac only
.PHONY : xcode
xcode: $(BUILD_DIR)
	cd $(BUILD_DIR); \
	cmake -G Xcode ..; \
	cp build/README.md .

# Cross-compile for Windows
.PHONY : windows
windows: $(BUILD_DIR)
	cd $(BUILD_DIR); \
	cmake -DCMAKE_TOOLCHAIN_FILE=../tools/Toolchain-mingw32.cmake -DCMAKE_BUILD_TYPE=Release ..; \
	cp build/README.md .

# Build the documentation using doxygen
.PHONY : documentation
documentation: $(BUILD_DIR)
	cd $(BUILD_DIR); \
	cmake -DDOCUMENTATION=1 ..; cd ..; \
	doxygen build/doxygen.conf; \
	cp build/README.md .

# Clean out the build directory
.PHONY : clean
clean:
	rm -rf $(BUILD_DIR)/*
