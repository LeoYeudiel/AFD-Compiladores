//Para poder leer un archivo y compilar
document.getElementById("archivo").addEventListener('change', (e) =>{
  const archivo = e.target.files[0];
  console.log(archivo.name);
  if (archivo) {
    if (archivo.name.endsWith('.psc')) {
      const leer = new FileReader();
      leer.onload = (e) => {
        const contenidoArchivo = e.target.result;
        document.getElementById('progra').value = contenidoArchivo;
      }
      leer.readAsText(archivo);
    } else {
      console.error("No es un archivo para cargar");
    }
  }
})