<template>
  <table>
    <tbody>
    <tr v-for="item in tableItems" :key="item.id">
      <th>{{ item.id }}</th>
      <th>{{ item.type }}</th>
      <th>{{ item.time }}</th>
    </tr>
    <tr v-for="incident in incidents" :key="incident.id">
      <td>{{ incident.id }}</td>
      <td>{{ incident.sensorStatus }}</td>
      <td>{{ incident.eventTime }}</td>
    </tr>
    </tbody>
  </table>
</template>

<script setup>
defineProps({
  tableItems: {
    type: Object,
    required: true,
  },
  incidents: {
    type: Object,
    required: true,
  }
})
</script>

<style scoped>
table {
  margin: auto auto;
  border: 2px solid mediumspringgreen;
  border-radius: 12px;
  padding: 12px;

  th {
    border-bottom: 2px solid white;
  }

  th,td {
    padding: 12px;
  }
}
</style>