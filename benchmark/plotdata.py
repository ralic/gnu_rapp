#!/usr/bin/python

#  Copyright (C) 2005-2010, Axis Communications AB, LUND, SWEDEN
#
#  This file is part of RAPP.
#
#  RAPP is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published
#  by the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  You can use the comments under either the terms of the GNU Lesser General
#  Public License version 3 as published by the Free Software Foundation,
#  either version 3 of the License or (at your option) any later version, or
#  the GNU Free Documentation License version 1.3 or any later version
#  published by the Free Software Foundation; with no Invariant Sections, no
#  Front-Cover Texts, and no Back-Cover Texts.
#  A copy of the license is included in the documentation section entitled
#  "GNU Free Documentation License".
#
#  RAPP is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License and a copy of the GNU Free Documentation License along
#  with RAPP. If not, see <http://www.gnu.org/licenses/>.


#####################################################
#  RAPP benchmark data plotter.
#####################################################

import sys, datetime

# The default name of the data file
DATAFILE = 'benchmarkdata.py'

# The name of the output plot file
PLOTFILE = 'benchmarkplot.html'

# The auto-generated tag string.
TAGSTRING = 'Auto-generated by RAPP Benchmark on'

# The bar plot palette
PLOTCOLORS = ['#66C2A5', '#FC8D62', '#8DA0CB', '#E78AC3',
              '#A6D854', '#FFD92F', '#D8A75E', '#A0A0A0']

# Get the score string for a value
def get_score(val):
    tab = ['T', 'G', 'M', 'k']
    sfx = ''

    for k in range(4):
        scale = 10 ** (3*(3 - k + 1))
        if val >= scale:
            val /= scale
            sfx  = tab[k]
            break

    return '%6.2f%c' % (val, sfx)


# Generate the HTML plot.
def plot(fname, data, size, info):
    html = """
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN"
                      "http://w3.org/TR/html4/strict.dtd">
<html>
  <head>
    <style>
      body, h1, td, th {
        font-family: georgia, verdana, geneva, arial, helvetica, sans-serif;
      }
      body {
        margin: 2em;
      }
      h1 {
        font-size: 18pt;
        text-align: center;
      }
      div.footer {
        font-style: italic;
        font-size: 8pt;
      }
      table.legend {
        font-size:  10pt;
        float: right;
        border: 1px solid black;
        margin: 0 0 1em 1em;
      }
      table.legend tr td div {
        width: 1em;
        border: 1px solid black;
      }
      table.legend tr td + td {
        width: auto;
      }
      table.plot {
        clear: both;
        margin: 2em 0 2em 0;
        white-space: nowrap;
      }
      table.plot tr td {
        font: 10pt monospace;
      }
      table.plot td + td + td {
        text-align: right;
      }
      table.plot thead th {
        font-size: 12pt;
        font-weight: bolder;
        text-align: left;
      }
      table.plot th + th {
        text-align: center;
      }
      span.desc {
        font-size: 8pt;
      }
      div.bg {
        border: 1px solid black;
      }
      div.bar {
        border-right: 1px solid black;
      }
    </style>
    <title>RAPP Benchmark</title>
  </head>
  <body>
  <h1>RAPP Benchmark</h1>
"""
    html += '<em>%s<br>Image size is %dx%d pixels</em>' % \
            (info, size[0], size[1])
    html += '<table>\n'

    body  = '<table class="plot">\n' + \
            '<thead><th>Function</th>' + \
            '<th>Execution Speed</th>' + \
            '<th>Pix/Sec</th></thead>'
    old   = ''
    color = -1

    # Compute the maximum value
    top = max([test[2] for test in data])

    # Plot all data
    for test in data:
        name = test[0]
        desc = test[1]
        val  = test[2]

        if desc:
            desc = '(' + desc + ')'

        # Update the color index
        if name != old:
            color = (color + 1) % len(PLOTCOLORS)
            old   = name

        # Plot the function name
        body += '<tr><td>%s <span class="desc">%s</span></td>' \
                '<td style="width: 100%%">\n' % \
                (name, desc)

        # Plot the full bar outline
        body += '<div class="bg" style="width: 100%%">\n'

        # Plot the filled bar
        body += '<div class="bar"' + \
                'style="background-color: %s; width: %.2f%%">\n' % \
                (PLOTCOLORS[color], 100.0*val / top)

        # Plot the score
        body += '<br></div></div>' \
                '</td><td>%s</td></tr>\n' % \
                get_score(val)

    # Add the footer
    body += '</table><div class="footer">%s %s</div></body></html>' % \
             (TAGSTRING, datetime.datetime.now().ctime())

    # Add the plot body
    html += body

    # Write the html plot to file
    out = open(fname, 'wb')
    out.write(html)
    out.close()


# Print the usage string
def usage():
    print 'Usage: ' + sys.argv[0] + ' [benchmark data file]'


# Main function
def main():
    # Determine the data file name
    fname = DATAFILE
    if len(sys.argv) > 1:
        fname = sys.argv[1]

    # Open the data file
    try:
        infile = open(fname, 'rb')
    except:
        usage()
        sys.exit(1)

    try:
        # Parse the data file
        exec infile

        # Generate the HTML plot
        plot(PLOTFILE, data, size, build)

        # Close the data file
        infile.close()

        print 'Plot written to ' + PLOTFILE

    except:
        print 'Invalid data file.'
        usage()
        infile.close()
        sys.exit(1)

    infile.close()


main()
