#!/usr/bin/env bash

javac -d . *.java
jar cfe word-search.jar word_search.Main word_search/*.class
java -jar word-search.jar ../examples/c-word-search
