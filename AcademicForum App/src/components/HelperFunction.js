export const DataToSelectOptions =(dataList, nameKey , idKey) => {
    return dataList?.map((item) => ({
        label: item[nameKey],
        value: item[idKey],
    }));
};

export const dateFormate = (time) => {
    // const timestamp = "2023-06-14T13:27:13.064Z";
    const date = new Date(time);

    const year = date.getFullYear();
    const month = String(date.getMonth() + 1).padStart(2, '0');
    const day = String(date.getDate()).padStart(2, '0');

    const formattedDate = `${year}-${month}-${day}`;
    return formattedDate;
};