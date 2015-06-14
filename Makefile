BUILD_DIR = build

$(BUILD_DIR_):
	-mkdir $(BUILD_DIR_) 2>/dev/null
	-cd $(BUILD_DIR); rm -rf *

# The release target will perform additional optimization
.PHONY : release
release: $(BUILD_DIR)
	cd $(BUILD_DIR); \
	cmake -DCMAKE_BUILD_TYPE=Release ..; \
	cp README.md ..

# Enables CuTest unit testing
.PHONY : debug
debug: $(BUILD_DIR)
	cd $(BUILD_DIR); \
	cmake -DTEST=1 ..; \
	cp README.md ..

# For Mac only
.PHONY : xcode
xcode: $(BUILD_DIR)
	cd $(BUILD_DIR); \
	cmake -G Xcode ..; \
	cp README.md ..

# Cross-compile for Windows
.PHONY : windows
windows: $(BUILD_DIR)
	cd $(BUILD_DIR); \
	cmake -DCMAKE_TOOLCHAIN_FILE=../tools/Toolchain-mingw32.cmake -DCMAKE_BUILD_TYPE=Release ..; \
	cp README.md ..

# Build the documentation using doxygen
.PHONY : documentation
documentation: $(BUILD_DIR)
	cd $(BUILD_DIR); \
	cmake -DDOCUMENTATION=1 ..; \
	cp README.md ..; \
	cd ..; \
	doxygen build/doxygen.conf; \
	cd documentation; rm -rf *; \
	cd ../build; cp -r documentation/html/* ../documentation;

# Clean out the build directory
.PHONY : clean
clean:
	rm -rf $(BUILD_DIR)/*
