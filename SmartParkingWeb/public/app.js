import { initializeApp } from "https://www.gstatic.com/firebasejs/9.8.4/firebase-app.js"
import { getDatabase, ref, onValue, get } from "https://www.gstatic.com/firebasejs/9.8.4/firebase-database.js"

const firebaseConfig = {
  apiKey: "AIzaSyC3aKF8EolxZkUz06NkmDVP-NjLKt2eo1E",
  authDomain: "sd-cesar-c642a.firebaseapp.com",
  databaseURL: "https://sd-cesar-c642a-default-rtdb.firebaseio.com",
  projectId: "sd-cesar-c642a",
  storageBucket: "sd-cesar-c642a.appspot.com",
  messagingSenderId: "733241428280",
  appId: "1:733241428280:web:c35b3090fdcdcedc1699cd",
  measurementId: "G-F5RJGRRSZ7"
};

document.addEventListener('DOMContentLoaded', function() {
  const app = initializeApp(firebaseConfig);                                                               
  const database = getDatabase(app);

  get(ref(database, `/maxVagas`)).then((snapshot) => {
    if (snapshot.exists()) {
      const maxVagas = snapshot.val();

      const vagasCountRef = ref(database, '/vagas');
      onValue(vagasCountRef, (snapshot) => {
        const data = snapshot.val();
        console.log(data);

        const occupiedEl = document.querySelector('#occupied');
        occupiedEl.textContent = data.toString();

        const availableEl = document.querySelector('#available');
        availableEl.textContent = (maxVagas - data).toString();

        const totalEl = document.querySelector('#total');
        totalEl.textContent = maxVagas.toString();
      });

      const intensityRef = ref(database, '/intensidade');
      onValue(intensityRef, (snapshot) => {
        const intensidade = snapshot.val();
        const intensidadeEl = document.querySelector('#intensity');
        intensidadeEl.textContent = intensidade.toString();
      });
    } else {
      console.log("No data available");
    }
  }).catch((error) => {
    console.error(error);
  });
});