import { initializeApp } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-app.js";
import { getAnalytics } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-analytics.js";
import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-database.js";

const firebaseConfig = {
    apiKey: "AIzaSyBFdJxALqcneH2pN117qCtmXjingMi79Pw",
    authDomain: "sensor-temp-c8608.firebaseapp.com",
    databaseURL: "https://sensor-temp-c8608-default-rtdb.firebaseio.com",
    projectId: "sensor-temp-c8608",
    storageBucket: "sensor-temp-c8608.firebasestorage.app",
    messagingSenderId: "890889952750",
    appId: "1:890889952750:web:715ab1f85030e04c802ff",
    measurementId: "G-10TCT7PYRJ"
};

const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);
const db = getDatabase(app);

const tempRef = ref(db, 'mediciones/temperatura');
const humRef = ref(db, 'mediciones/humedad');

const tempEl = document.getElementById('temp-val');
const humEl = document.getElementById('hum-val');
const statusEl = document.getElementById('temp-status');

onValue(tempRef, (snapshot) => {
    const temp = snapshot.val();
    if (temp !== null && tempEl) {
        tempEl.innerText = `${temp} °C`;
        if (statusEl) {
            if (temp > 35) {
                statusEl.innerText = "Temperatura Alta (LED Rojo)";
                statusEl.className = "status alerta";
            } else {
                statusEl.innerText = "Temperatura Normal (LED Verde)";
                statusEl.className = "status normal";
            }
        }
    }
});

onValue(humRef, (snapshot) => {
    const hum = snapshot.val();
    if (hum !== null && humEl) {
        humEl.innerText = `${hum} %`;
    }
});