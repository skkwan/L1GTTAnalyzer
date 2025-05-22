# parseYaml.py
import os
import yaml

config_file = open("datasetConfig.yml")

config = yaml.safe_load(config_file)

CMSSW_BASE = os.getenv('CMSSW_BASE') + "/"

print("Found CMSSW_BASE as", CMSSW_BASE)

for eraType in ["minbias", "signal"]:
   for d in config[eraType]["datasets"]:

      dir = "crabJobConfigs/" + str(config[eraType]["year"]) + "/" 
      os.system("mkdir -p " + dir)

      newCRABConfFile  = dir + d + "_" + config[eraType]["prodtag"] + "_cfg.py"  
      print(newCRABConfFile)

      requestName      = "test_" + config["requestname"] + "_" + d + "_" + config[eraType]["prodtag"]  
      crabPythonScript = CMSSW_BASE + config[eraType]["script"]      # crab python script: one per era (will say which jmeCorrection to use)
      outputFile       = config[eraType]["outputName"]  # output name e.g. analyzer.root, that CRAB should expect
      dasName          = config[eraType]["datasets"][d] # DAS name
      inputDBS         = config[eraType]["inputDBS"]    # inputDBS
      outputTag        = 'Analyzer_' + d + "_" + config[eraType]["prodtag"]  
      nFilesPerJob     = config[eraType]["nFilesPerJob"][d]

      # Make one new template file per dataset
      with open('crab_template_cfg.py', 'r') as templatefile:
         t1 = templatefile.read()
         with open(newCRABConfFile, 'w+') as writefile:

            t2 = t1.replace('REQUEST_NAME',            requestName)
            t3 = t2.replace('CRAB_PYTHON_SCRIPT_PATH', crabPythonScript)
            t4 = t3.replace('OUTPUT_FILE_NAME',        outputFile)
            t5 = t4.replace('DAS_NAME',                dasName)
            t6 = t5.replace('INPUT_DBS',               inputDBS)
            t7 = t6.replace('OUTPUT_TAG',              outputTag)
            t8 = t7.replace('N_FILES_PER_JOB',         str(nFilesPerJob))

            writefile.write(t8)

#-------------------------------------------------------#
