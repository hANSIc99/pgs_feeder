1 #!/bin/bash
value=$(<test.json)
valgrind --leak-check=yes ./pgs_feeder "$value"
