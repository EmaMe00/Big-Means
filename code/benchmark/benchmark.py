import os

file_path = "3dSpatialNetwork.txt"
dataset = "../../datasets/3D_spatial_network.txt"
main_path = "../bin/main"
iteration_for_setting = 5

header = "Grandezza campione,Numero di centroidi,Iterazioni di Big Means,Tempo trascorso,LOSS FINALE,Ea\n"

with open(file_path, "a") as file:
    file.write(header)

    parameters = [
        (100000, 1, 1, ',', 'f', 2, 5, 49132980),
        (100000, 1, 1, ',', 'f', 3, 5, 22778180),
        (100000, 1, 1, ',', 'f', 5, 5, 8825740),
        (100000, 1, 1, ',', 'f', 10, 5, 2566610),
        (100000, 1, 1, ',', 'f', 15, 5, 1270690),
        (100000, 1, 1, ',', 'f', 20, 5, 808650),
        (100000, 1, 1, ',', 'f', 25, 5, 592590),
    ]

    for param in parameters:
        command = (
            f"{main_path} -f \"{dataset}\" "
            f"-s {param[0]} -r {param[1]} -c {param[2]} -d '{param[3]}' -t '{param[4]}' "
            f"-k {param[5]} -i {param[6]} -E {param[7]} -F \"{file_path}\" -B"
        )

        for _ in range(iteration_for_setting):
            os.system(command)
        
        file.write(header)
