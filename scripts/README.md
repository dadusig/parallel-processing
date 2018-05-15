# Automation Scripts   

## Σειριακό πρόγραμμα - fack script

```sh
#first move fack file to on of $PATH folders and make it executable
#chmod +x fack

fack -p path/to/code -f filename {-O3, -O0} {--big, --small} {-ALL}

#for example
#cd parallel-processing
#fack -p 1b/ --big -O3
#fack -p 1c/ --big -O3 -lblas
```

### Parameters

| parameter | type    | description                                      |
| --------- | ------- | ------------------------------------------------ |
| `-p /path/to/code`     | optional  | default value: ./ (you are inside code directory)        |
| `-f filename`  | optional | file to compile. default value: lif1d.c          |
| `-ALL` | optional  | adds -DALL_RESULTS to execution                            |
| `--small`  | optional | executes for n=("200" "400" "600" "800" "1000") |
| `--big`  | optional | executes for n=("1000" "2000" "3000" "4000" "5000") |

Αν κανένα από τα --small or --big δεν οριστεί κατά την κλήση τότε εκτελεί για:

n=("200" "400" "600" "800" "1000" "2000" "3000" "4000" "5000")

με τα αντίστοιχα r:

r=("60"  "120" "200" "250"  "350"  "700" "1000" "1300" "1600")
