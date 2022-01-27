#!/bin/bash

# Create temporary directory
dir=`mktemp -d`

# Create directory structure
cd $dir
mkdir a a/b a/c a/b/d a/b/e a/b/f

# Populate directory with files
touch a/g a/h # Create two empty files
ln -s a/g a/i
ln    a/h a/j

# Full listing of directory "a"
ls -la a

# Cleanup
rm -rf $dir
