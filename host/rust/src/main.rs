use std::path::PathBuf;

mod async_add;
mod state;
mod sync_add;

#[async_std::main]
async fn main() -> anyhow::Result<()> {
    let x = 1;
    let y = 2;
    let component: PathBuf = "../../adder/adder.wasm".into();
    let sum1 = async_add::add(component.clone(), x, y).await?;
    let sum2 = sync_add::add(component, x, y)?;
    assert_eq!(sum1, sum2);
    println!("{} + {} = {sum1}", x, y);
    Ok(())
}
