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
