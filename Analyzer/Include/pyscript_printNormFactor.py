import argparse
import json

parser = argparse.ArgumentParser(description='Argparse Tutorial')

parser.add_argument('--json', help='Input JSON file name')
parser.add_argument('--lumi', type=float, help='Integrated luminosity')

args = parser.parse_args()

with open(args.json) as fullSampleInfo_file:
    fullSampleInfo = json.load(fullSampleInfo_file)

print "Integrated luminosity = %lf (pb)" % args.lumi

print "sampleType\tcross section\tsum(weight)\tnorm.factor"
for sample in fullSampleInfo["Sample"]:
    if sample["isMC"]:
        normFactor = (args.lumi * sample["xSec"]) / sample["sumWeight"]
        print "%s\t%lf\t%.1lf\t%lf" % (sample["tag"], sample["xSec"], sample["sumWeight"], normFactor)
