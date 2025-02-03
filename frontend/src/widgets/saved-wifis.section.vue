<script setup lang="ts">
import { computed, ref } from 'vue'
import ContentSection from '@/components/content-section.vue'
import List from '@/components/list.vue'
import Accordion from '@/components/accordion.vue'
import WifiCredentialsForm from '@/components/wifi-credentials-form.vue'
import { useDeleteWifi, useEditWifi, useSaveWifi, useWifiList } from '@/data/wifi'
import type { WifiCredentials } from '@/data/config_api'

const focusedEntity = ref<string | null>()

const { data } = useWifiList()
const deleteWifi = useDeleteWifi()
const editWifi = useEditWifi()
const saveWifi = useSaveWifi()

const listAdopted = computed(() => {
  const items = (data.value || []).map((item, index) => ({
    slot: 'wifi',
    key: index.toString(),
    ...item,
  }))
  items?.push({ slot: 'new', key: 'new', ssid: '' })
  return items
})

const handleClickEntity = (value: string) => {
  if (focusedEntity.value == value) {
    focusedEntity.value = null
    return
  }

  focusedEntity.value = value
}

const handleDelete = (index: number) => {
  deleteWifi({ index })
}
const handleEdit = (index: number, data: WifiCredentials) => {
  editWifi({
    index,
    ...data,
  })
}
const handleSave = (data: WifiCredentials) => {
  saveWifi(data)
}
</script>

<template>
  <ContentSection
    title="Saved networks"
    subtitle="Networks are listed in order of connection priority"
  >
    <List :items="listAdopted" @click="handleClickEntity">
      <template #wifi="{ key, ssid, password }">
        <Accordion :is-open="focusedEntity === key">
          <template #summary>
            <p>{{ `${Number(key) + 1}.  ${ssid}` }}</p>
          </template>
          <template #details>
            <WifiCredentialsForm
              delitable
              :default-values="{ ssid, password }"
              @delete="handleDelete(Number(key))"
              @save="(data) => handleEdit(Number(key), data)"
            />
          </template>
        </Accordion>
      </template>
      <template #new>
        <Accordion :is-open="focusedEntity === 'new'">
          <template #summary>
            <p>Save new network</p>
          </template>
          <template #details>
            <WifiCredentialsForm @save="handleSave" />
          </template>
        </Accordion>
      </template>
    </List>
  </ContentSection>
</template>
