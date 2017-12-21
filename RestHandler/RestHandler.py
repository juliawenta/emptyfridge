import sys
import requests
import json
from pprint import pprint


ingridientsFileName='C:/Users/Wenta/Documents/emptyfridge/RestHandler/resources/ingridients.txt'
recipeJsonFileName='C:/Users/Wenta/Documents/emptyfridge/RestHandler/resources/recipe.json'
resultFileName='C:/Users/Wenta/Documents/emptyfridge/RestHandler/resources/result.txt'
apiKey = '3c235b65ed5ac81c2418262fb477b45d' #insert your  api key here

def readFile(fileName):
	file = open(fileName,'r') 
	return file.read()

def saveFile(fileName, fileBody):
    file = open(fileName,'w') 
    file.write(fileBody)
    file.close()

def getRecipe():
    ingridients = readFile(ingridientsFileName)
    url = 'https://api.edamam.com/search?q='+ ingridients + '&app_id=54edf467&app_key=' + apiKey + '&from=0&to=3&calories=gte%20591,%20lte%20722&health=alcohol-free'
    res = requests.get(url)
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
    recipeNumber = sys.argv[2]
    if type == 'url':
        res = getRecipeUrl(recipeNumber)        
        saveFile(resultFileName, res)
    elif type == 'image':
        res = getRecipeImageUrl(recipeNumber)
        saveFile(resultFileName, res)
    elif type == 'ingredient':
        res = getRecipeIngredientLines(recipeNumber)
        saveFile(resultFileName, res)
    elif type == 'title':
        res = getRecipeIngredientLines(recipeNumber)
        saveFile(resultFileName, res)

getRecipe()
parseArgv()
