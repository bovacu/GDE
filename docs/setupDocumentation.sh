pip install sphinx
pip install exhale
pip install myst_parser

cd $PWD && sphinx-quickstart << EOF
n
GDE
Borja Vazquez Cuesta
0.5.0
en
EOF

cat >conf.py <<EOL
# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'GDE'
copyright = '2022, Borja Vazquez Cuesta'
author = 'Borja Vazquez Cuesta'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
   # there may be others here already, e.g. 'sphinx.ext.mathjax'
   'breathe',
   'exhale',
   'sphinx_rtd_theme',
   'myst_parser'
]

# Setup the breathe extension
breathe_projects = {
   "My Project": "./_doxygen/xml"
}
breathe_default_project = "My Project"

# Setup the exhale extension
exhale_args = {
   # These arguments are required
   "containmentFolder":     "./api",
   "rootFileName":          "library_root.rst",
   "doxygenStripFromPath":  "..",
   # Heavily encouraged optional argument (see docs)
   "rootFileTitle":         "Library API",
   # Suggested optional arguments
   "createTreeView":        True,
   # TIP: if using the sphinx-bootstrap-theme, you need
   # "treeViewIsBootstrap": True,
   "exhaleExecutesDoxygen": True,
   "exhaleDoxygenStdin":    "INPUT = ../engine/include"
}

myst_enable_extensions = [
   "amsmath",
   "colon_fence",
   "deflist",
   "dollarmath",
   "fieldlist",
   "html_admonition",
   "html_image",
   "replacements",
   "smartquotes",
   "strikethrough",
   "substitution",
   "tasklist",
]

# Tell sphinx what the primary language being documented is.
primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'cpp'

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = "sphinx_rtd_theme"

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']
EOL

cat >index.rst <<EOL
.. GDE documentation master file, created by
   sphinx-quickstart on Thu Jun 16 11:55:40 2022.
   You can adapt this file completely to your liking, but it should at least
   contain the root  directive.

Welcome to GDE's documentation!
===============================

.. toctree::
   :maxdepth: 1
   :caption: Contents:

   api/library_root
   internalDocs/internalDocs



Indices and tables
==================

* :ref:\`genindex\`
* :ref:\`search\`
EOL

mkdir internalDocs
touch internalDocs/internalDocs.md

cat >internalDocs/internalDocs.md <<EOL
\`\`\`{include} ../EngineDocs.md
\`\`\`
EOL

