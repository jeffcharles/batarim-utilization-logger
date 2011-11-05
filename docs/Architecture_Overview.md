# Architecture Overview #

Batarim's architecture breaks down into five major groupings of classes:

1. Data sources
2. The usage reporter
3. Process collections
4. The view model
5. Views

## Data sources ##

Data sources are responsible for retrieving the information that feeds into the
view model. Generally speaking, this information is retrieved by calling into
Win32 APIs on Windows and reading proc files or calling into library APIs on
Linux. Typically information is returned as primitive types. Unless there is a
good reason, data sources should be completely independent of each other. The
process collections are a special kind of data source that will be reviewed
later.

## The usage reporter ##

The usage reporter is used to coordinate retrieving and calculating CPU usage
percentages. First an initial reading needs to be taken, then a pause of at
least one second needs to occur, then a second reading is taken. The first
reading is subtracted from the second and divided over the length of the pause
to determine the usage percentage. Since pausing the program once for each
process and processor would be unacceptable, the usage reporter's analyze
method is responsible for initializing each CPU usage related data source,
pausing, and then calling into methods on each data source for updating their
data.

## Process collections ##

Batarim includes four process collections:

1. The raw process collection
2. The formatted process collection
3. The aggregated process collection
4. The process tree

The raw process collection is responsible for storing raw process times, amount
of RAM used, and the process's parent process. The formatted process collection
is responsible for storing each process's CPU usage percentage, RAM usage
percentage, and providing access to the identifier of the parent process. The
aggregated process collection is similar to the formatted process collection
except that it treats all processes with the same name as one process. Finally,
the process tree is responsible for determining which processes are considered
top-level processes and assigning all non top-level processes' CPU and RAM
usage to their top-level ancestor.

## The view model ##

The view model is responsible for retrieving data from all data sources,
grouping related data together, and assigning a human readable name for each
piece of data and grouping. It takes a displayer and when told to display, it
iterates over itself and displays each grouping and piece of data using the
provided displayer. This component's design will probably be subject to review
and change in a future version to reduce the number of hard dependencies,
simplify the display interface, and simplify the way data is packaged for the
views.

## Views ##

Each view contains an executable program and one or more displayers. The
displayers are used to provide rendering behaviour to the view model's display
functions. The executable is responsible for asking for the view model and
plugging a displayer in and asking the view model to display. It is also
possible to change the displayer in a view model without regenerating the view
model.
