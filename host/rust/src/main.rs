use std::path::PathBuf;

mod state;
mod sync_add;

fn main() -> anyhow::Result<()> {
    let x = 1;
    let y = 2;
    let component: PathBuf = "../../adder/adder.wasm".into();
    let sum = sync_add::add(component, x, y)?;
    println!("{} + {} = {sum}", x, y);
    Ok(())
}