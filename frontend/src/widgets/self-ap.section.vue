<script setup lang="ts">
import WifiCredentialsForm from '@/components/wifi-credentials-form.vue'
import type { WifiCredentials } from '@/data/config_api'
import { useGetSelfAp, useSetSelfAp } from '@/data/wifi'

const { data, isLoading } = useGetSelfAp()

const setApCreds = useSetSelfAp()

const handleSave = (creds: WifiCredentials) => {
  setApCreds(creds)
}
</script>

<template>
  <div class="flex flex-col gap-1">
    <h3>Edit self access point</h3>
    <WifiCredentialsForm :default-values="data" :is-loading="isLoading" @save="handleSave" />
  </div>
</template>
