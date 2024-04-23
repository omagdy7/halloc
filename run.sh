#!/bin/sh

# Output directories
OUTDIR="target"
DBGDIR="$OUTDIR/debug"
RELDIR="$OUTDIR/release"

# Binary names
DEBUG_BINARY="$DBGDIR/bin-debug"
RELEASE_BINARY="$RELDIR/bin-release"

# Function to print usage
print_usage() {
	echo "Usage: $0 [debug|release]"
}

# Function to run the binary
run_binary() {
	binary="$1"
	make
	if [ -f "$binary" ]; then
		"$binary"
	else
		echo "Error: $binary does not exist."
	fi
}

# Check if an argument is provided
if [ "$#" -ne 1 ]; then
	print_usage
	exit 1
fi

# Run the appropriate binary based on the argument
case "$1" in
debug)
	run_binary "$DEBUG_BINARY"
	;;
release)
	run_binary "$RELEASE_BINARY"
	;;
*)
	print_usage
	exit 1
	;;
esac
