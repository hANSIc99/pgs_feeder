1 #!/bin/bash
value=$(<test.json)
./pgs_feeder "$value"
