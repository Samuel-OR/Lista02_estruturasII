import math

def qtdmovimentos(lista1, lista2):
	cont = 0
	for i in range(0, len(lista2)):
		if(lista1[i] != lista2[i]):
			cont+=1
	return cont

def validarEstado(lista):
	if(lista in Anteriores):
		return False

	if(qtdmovimentos(lista, aux)!=1):
		return False

	Anteriores.append(lista)
	return True

def gerarPossibilidades():
	lista = []
	for x in range(1, 3+1):
		for y in range(1, 3+1):
			for z in range(1, 3+1):
				for w in range(1, 3+1):
					lista.append([x,y,z,w])
	return lista



	


if __name__ == '__main__':
	Todos = gerarPossibilidades()
	possibilidades = math.pow(3,4)

	validades = 