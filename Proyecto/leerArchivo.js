//Para poder leer un archivo y compilar
document.getElementById("archivo").addEventListener('change', (e) =>{
  const archivo = e.target.files[0];
  if (archivo) {
    const leer = new FileReader();
    leer.onload = (e) => {
      const contenidoArchivo = e.target.result;
      document.getElementById('progra').value = contenidoArchivo;
    }
    leer.readAsText(archivo);
  }
})