1 #!/bin/bash
value=$(<test.json)
valgrind --leak-check=yes --track-origins=yes ./pgs_feeder "$value"
