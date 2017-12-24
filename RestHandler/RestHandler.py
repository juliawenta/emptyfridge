import sys
import requests
import json
from pprint import pprint
import shutil
import codecs
'''
sys.stdout = codecs.getwriter("iso-8859-1")(sys.stdout, 'xmlcharrefreplace')

if sys.stdout.encoding != 'cp850':
  sys.stdout = codecs.getwriter('cp850')(sys.stdout.buffer, 'strict')
if sys.stderr.encoding != 'cp850':
  sys.stderr = codecs.getwriter('cp850')(sys.stderr.buffer, 'strict')
  '''
ingridientsFileName='C:/Users/Wenta/Documents/emptyfridge/RestHandler/resources/ingridients.txt'
recipeJsonFileName='C:/Users/Wenta/Documents/emptyfridge/RestHandler/resources/recipe.json'
resultFileName='C:/Users/Wenta/Documents/emptyfridge/RestHandler/resources/result.txt'
apiKey = '' #insert your  api key here

from shutil import copyfile
copyfile('C:/Users/Wenta/Documents/emptyfridge/ingr.txt', 'C:/Users/Wenta/Documents/emptyfridge/RestHandler/resources/ingridients.txt')

def readFile(fileName):
	file = open(fileName,'r')
	with open('C:/Users/Wenta/Documents/emptyfridge/ingr.txt') as f:
		lines = f.readlines()
	with open(ingridientsFileName, 'w') as f1:
		f1.writelines(lines)
	return file.read()

def saveFile(fileName, fileBody):
    file = open(fileName,'w', encoding='utf-8')
    file.write(fileBody)

    file.close()

def getRecipe():
    ingridients = readFile(ingridientsFileName)
    url = 'https://api.edamam.com/search?q='+ ingridients + '&app_id=54edf467&app_key=' + apiKey + '&from=0&to=3&calories=gte%20591,%20lte%20722&health=alcohol-free'
    res = requests.get(url) #we use request library to call edamam.com service to get json with list of recipes
    saveFile(recipeJsonFileName, res.text)

   
def getRecipeImageUrl(recipeNumber):
    with open(recipeJsonFileName) as data_file:    
        data = json.load(data_file)
        return data['hits'][recipeNumber]['recipe']['image']

def getRecipeIngredientLines(recipeNumber):
    with open(recipeJsonFileName) as data_file:    
        data = json.load(data_file)
        return data['hits'][recipeNumber]['recipe']['ingredientLines']

def getRecipeUrl(recipeNumber):
    with open(recipeJsonFileName) as data_file:    
        data = json.load(data_file)
        return data['hits'][int(recipeNumber)]['recipe']['url']

def getRecipeTitle(recipeNumber):
    with open(recipeJsonFileName) as data_file:    
        data = json.load(data_file)
        return data['hits'][int(recipeNumber)]['recipe']['label']


def parseArgv():
    type = sys.argv[1]
    if type == 'url':
        recipeNumber = sys.argv[2]
        res = getRecipeUrl(recipeNumber)        
        saveFile(resultFileName, res)
    elif type == 'image':
        recipeNumber = sys.argv[2]
        res = getRecipeImageUrl(recipeNumber)
        saveFile(resultFileName, res)
    elif type == 'ingredient':
        recipeNumber = sys.argv[2]
        res = getRecipeIngredientLines(recipeNumber)
        saveFile(resultFileName, res)
    elif type == 'title':
        recipeNumber = sys.argv[2]
        res = getRecipeIngredientLines(recipeNumber)
        saveFile(resultFileName, res)
    elif type == 'recipe':
        getRecipe()

#sys.argv[1]='url'
#sys.argv[2]='1'


parseArgv()
