#!/bin/bash
./RSSReader -json -dom -customer stephan_company -keyword trump -keyword clinton -keyword syria -keyword russia edition.cnn.com/rss/cnn_topstories.rss | ./pgs_feeder_1.0 
