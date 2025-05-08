<template>
  <div>
    <h1 class="header">Motion Sensor Logs</h1>
  </div>
  <table-component :table-items="tableItems" :incidents="incidents"/>
</template>

<script setup>
import {onMounted, ref} from "vue";
import TableComponent from "@/components/TableComponent.vue";

const incidents = ref({});
const tableItems = ref({
  item: {
    id: "Incident ID",
    type: "Incident",
    time: "Time",
  }
});

onMounted(async() => {
  await getIncidents();
  console.log(incidents.value)
});

async function getIncidents() {
  const url = "http://localhost:8080/api/motion";
  try {
    const response = await fetch(url);
    const json = await response.json();
    incidents.value = {};
    for (let item of json) {
      incidents.value = {
        ... incidents.value,
        [item.id]: item,
      };
    }
  } catch (Exception) {
    console.log(Exception);
  }
}

</script>

<style scoped>
.header {
  color: #4caf50;
  margin-bottom: 8px;
}
</style>
